"""The initial corpus download must not require generated build prerequisites."""

import os
from pathlib import Path
import shutil
import subprocess
import unittest

ROOT = Path(__file__).resolve().parents[2]


class PokedexFetchTargetTests(unittest.TestCase):
    @unittest.skipUnless(shutil.which("make") and shutil.which("arm-none-eabi-gcc"),
                         "make and the ARM toolchain required")
    def test_fetch_target_does_not_run_generation_or_build_tools(self):
        env = dict(os.environ)
        env.pop("MAKEFLAGS", None)
        env.pop("MFLAGS", None)
        result = subprocess.run(["make", "--no-print-directory", "-n", "fetch-pokedex-text"],
                                cwd=ROOT, env=env, capture_output=True, text=True)
        self.assertEqual(result.returncode, 0, result.stderr)
        self.assertEqual(result.stdout.strip(), "python3 tools/pokedex_kanji.py fetch")


if __name__ == "__main__":
    unittest.main()
