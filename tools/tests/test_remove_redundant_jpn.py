"""Conservative prefix migration and real renderer parity before any edits."""

from pathlib import Path
import shutil
import struct
import subprocess
import sys
import tempfile
import unittest

TOOLS = Path(__file__).resolve().parents[1]
sys.path.insert(0, str(TOOLS))
import remove_redundant_jpn as migration
from test_text_language_mode import engine_code


def verify_rows(root, rows):
    compiler = shutil.which("cc") or shutil.which("gcc")
    if compiler is None:
        raise RuntimeError("A host C compiler is required before editing; run in WSL with cc.")
    selected = [row for row in rows if row["action"] == "remove"]
    fixture = root / "tools/tests/fixtures"
    base = (fixture / "text_language_mode.c").read_text(encoding="utf-8")
    base = base.replace("int main(", "int fixture_main(").replace("sTrace[128]", "sTrace[8192]")
    program = base.replace("/* ENGINE_FUNCTIONS */", engine_code())
    program += (fixture / "jpn_prefix_migration.c").read_text(encoding="utf-8")
    with tempfile.TemporaryDirectory() as directory:
        directory = Path(directory)
        source, binary, corpus = (directory / name for name in ("test.c", "test", "corpus.bin"))
        source.write_text(program, encoding="utf-8")
        with corpus.open("wb") as handle:
            for row in selected:
                data = bytes.fromhex(row["encoded_hex"])
                handle.write(struct.pack("<I", len(data)) + data)
        subprocess.run([compiler, "-std=gnu17", "-O1", "-DTEST_LEGACY_MODE=0",
                        "-iquote", str(root / "include"), str(source), "-o", str(binary)], check=True)
        result = subprocess.run([str(binary), str(corpus)], capture_output=True, text=True)
        if result.returncode:
            failed = [selected[int(line.split()[1])] for line in result.stdout.splitlines()
                      if line.startswith("MISMATCH ")]
            details = "\n".join(f"{r.get('file')}:{r.get('line')} {r.get('label')}" for r in failed)
            raise RuntimeError("Renderer comparison failed; no edits applied.\n" + details + "\n" + result.stderr)
        print(result.stdout.strip(), flush=True)


class PrefixMigrationTests(unittest.TestCase):
    def test_asm_keeps_embedded_modes_and_commands(self):
        text = 'Script::\r\n\tlock\r\n\tmsgbox Text\r\n\trelease\r\nText::\r\n\t.string "{JPN}あ\\n"\r\n\t.string "{JPN}い$"\r\n'
        rows = list(migration.candidates(Path("scripts.inc"), text))
        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]["text"], r"{JPN}あ\n{JPN}い$")
        self.assertEqual(text[rows[0]["offset"]:][:5], "{JPN}")

    def test_c_adjacent_literals_and_comments(self):
        text = '/* _("{JPN}コメント") */\nconst u8 sText[] = _("{JPN}あ" "{ENG}A{JPN}い");\n'
        rows = list(migration.candidates(Path("test.c"), text))
        self.assertEqual(len(rows), 1)
        self.assertEqual(rows[0]["label"], "sText")
        self.assertEqual(rows[0]["text"], "{JPN}あ{ENG}A{JPN}い")

    def test_names_and_reused_fragments_retained(self):
        row = {"label": ".speciesName", "kind": "c", "field": "speciesName"}
        self.assertEqual(migration.policy(row, "test.h", {}, set(), set())[0], "retain")
        row = {"label": "Text", "kind": "asm"}
        self.assertEqual(migration.policy(row, "test.inc", {}, {"Text"}, {"Text"})[0], "retain")
        self.assertEqual(migration.policy(row, "test.inc", {}, {"Text"}, set())[0], "remove")

    def test_apply_only_prefix_preserves_mixed_newlines_and_is_idempotent(self):
        text = 'Text::\r\n\t.string "{JPN}あ\\n"\n\t.string "{ENG}A{JPN}い$"\r\n'
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            path = root / "test.inc"
            path.write_bytes(text.encode("utf-8"))
            rows = list(migration.candidates(path, text))
            for row in rows:
                row.update(file="test.inc", line=2, action="remove")
            migration.apply_rows(root, {path: text}, rows)
            after = path.read_bytes().decode("utf-8")
            self.assertEqual(after, text.replace("{JPN}", "", 1))
            self.assertEqual(list(migration.candidates(path, after)), [])

    def test_changed_source_is_not_overwritten(self):
        with tempfile.TemporaryDirectory() as directory:
            path = Path(directory) / "test.c"
            original = 'const u8 sText[] = _("{JPN}あ");'
            path.write_text(original + "// user edit", encoding="utf-8")
            row = next(migration.candidates(path, original))
            row.update(file="test.c", line=1, action="remove")
            with self.assertRaisesRegex(ValueError, "source changed"):
                migration.apply_rows(path.parent, {path: original}, [row])
            self.assertTrue(path.read_text(encoding="utf-8").endswith("// user edit"))

    def test_renderer_parity_and_mismatching_initial_space(self):
        if not (shutil.which("cc") or shutil.which("gcc")):
            self.skipTest("run in WSL with cc")
        charmap = migration.Charmap(TOOLS.parent / "charmap.txt")
        texts = ["{JPN}あいアヴ", "{JPN}10%で まひ\\nさわる", "{JPN}{R_BUTTON} とうろく",
                 "{JPN}{PLAYER}の ポケモン", "{JPN}あ{ENG}ABC{AUTO}い",
                 "{JPN}{EMOJI_UNDERSCORE}あ"]
        rows = [{"action": "remove", "encoded_hex": migration.encode(text, charmap).hex()} for text in texts]
        verify_rows(TOOLS.parent, rows)
        rows = [{"action": "remove", "encoded_hex": migration.encode("{JPN} {ENG}A", charmap).hex()}]
        with self.assertRaisesRegex(RuntimeError, "Renderer comparison failed"):
            verify_rows(TOOLS.parent, rows)


if __name__ == "__main__":
    unittest.main()
