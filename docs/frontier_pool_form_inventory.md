# Battle Frontier form inventory (2026-10-12)

This is an audit of `gBattleFrontierMons` after removing Flabebe. Each entry
below is a non-first member of a form species table that also has a dedicated
Frontier set. The first member (the default form) is omitted. The inventory is
not an exclusion list: regional, gender, cosmetic, and battle-only forms are
all included. No form below was removed as part of this change.

There are 102 alternate species across 78 form families. Each listed species
has three Frontier sets. Prefix each name below with `SPECIES_` for its C ID.

| Family | Alternate species in the pool |
| --- | --- |
| Raticate | RATICATE_ALOLA |
| Pichu | PICHU_SPIKY_EARED |
| Raichu | RAICHU_ALOLA |
| Sandslash | SANDSLASH_ALOLA |
| Ninetales | NINETALES_ALOLA |
| Dugtrio | DUGTRIO_ALOLA |
| Persian | PERSIAN_ALOLA |
| Arcanine | ARCANINE_HISUI |
| Golem | GOLEM_ALOLA |
| Rapidash | RAPIDASH_GALAR |
| Slowbro | SLOWBRO_GALAR |
| Slowking | SLOWKING_GALAR |
| Muk | MUK_ALOLA |
| Electrode | ELECTRODE_HISUI |
| Exeggutor | EXEGGUTOR_ALOLA |
| Marowak | MAROWAK_ALOLA |
| Weezing | WEEZING_GALAR |
| Tauros | TAUROS_PALDEA_COMBAT, TAUROS_PALDEA_BLAZE, TAUROS_PALDEA_AQUA |
| Articuno | ARTICUNO_GALAR |
| Zapdos | ZAPDOS_GALAR |
| Moltres | MOLTRES_GALAR |
| Typhlosion | TYPHLOSION_HISUI |
| Dudunsparce | DUDUNSPARCE_THREE_SEGMENT |
| Ursaluna | URSALUNA_BLOODMOON |
| Corsola | CORSOLA_GALAR |
| Castform | CASTFORM_SUNNY, CASTFORM_RAINY, CASTFORM_SNOWY |
| Deoxys | DEOXYS_ATTACK, DEOXYS_DEFENSE, DEOXYS_SPEED |
| Wormadam | WORMADAM_SANDY, WORMADAM_TRASH |
| Cherrim | CHERRIM_SUNSHINE |
| Gastrodon | GASTRODON_EAST |
| Rotom | ROTOM_HEAT, ROTOM_WASH, ROTOM_FROST, ROTOM_FAN, ROTOM_MOW |
| Dialga | DIALGA_ORIGIN |
| Palkia | PALKIA_ORIGIN |
| Giratina | GIRATINA_ORIGIN |
| Shaymin | SHAYMIN_SKY |
| Samurott | SAMUROTT_HISUI |
| Lilligant | LILLIGANT_HISUI |
| Basculin | BASCULIN_BLUE_STRIPED |
| Basculegion | BASCULEGION_F |
| Darmanitan | DARMANITAN_GALAR_STANDARD |
| Zoroark | ZOROARK_HISUI |
| Sawsbuck | SAWSBUCK_SUMMER, SAWSBUCK_AUTUMN, SAWSBUCK_WINTER |
| Stunfisk | STUNFISK_GALAR |
| Braviary | BRAVIARY_HISUI |
| Tornadus | TORNADUS_THERIAN |
| Thundurus | THUNDURUS_THERIAN |
| Landorus | LANDORUS_THERIAN |
| Kyurem | KYUREM_WHITE, KYUREM_BLACK |
| Keldeo | KELDEO_RESOLUTE |
| Meloetta | MELOETTA_PIROUETTE |
| Greninja | GRENINJA_BATTLE_BOND, GRENINJA_ASH |
| Meowstic | MEOWSTIC_F |
| Goodra | GOODRA_HISUI |
| Avalugg | AVALUGG_HISUI |
| Zygarde | ZYGARDE_10_AURA_BREAK, ZYGARDE_10_POWER_CONSTRUCT, ZYGARDE_50_POWER_CONSTRUCT, ZYGARDE_COMPLETE |
| Hoopa | HOOPA_UNBOUND |
| Decidueye | DECIDUEYE_HISUI |
| Oricorio | ORICORIO_POM_POM, ORICORIO_PAU, ORICORIO_SENSU |
| Lycanroc | LYCANROC_MIDNIGHT, LYCANROC_DUSK |
| Necrozma | NECROZMA_DUSK_MANE, NECROZMA_DAWN_WINGS |
| Magearna | MAGEARNA_ORIGINAL |
| Cramorant | CRAMORANT_GULPING, CRAMORANT_GORGING |
| Toxtricity | TOXTRICITY_LOW_KEY |
| Polteageist | POLTEAGEIST_ANTIQUE |
| Eiscue | EISCUE_NOICE |
| Indeedee | INDEEDEE_F |
| Morpeko | MORPEKO_HANGRY |
| Zacian | ZACIAN_CROWNED |
| Zamazenta | ZAMAZENTA_CROWNED |
| Eternatus | ETERNATUS_ETERNAMAX |
| Urshifu | URSHIFU_RAPID_STRIKE |
| Zarude | ZARUDE_DADA |
| Calyrex | CALYREX_ICE, CALYREX_SHADOW |
| Enamorus | ENAMORUS_THERIAN |
| Oinkologne | OINKOLOGNE_F |
| Maushold | MAUSHOLD_FOUR |
| Palafin | PALAFIN_HERO |
| Sinistcha | SINISTCHA_MASTERPIECE |

## Form-named first members

These 45 species are the first member of their form table. They are present
in the pool but are not counted among the 102 alternate entries above.

`DUDUNSPARCE_TWO_SEGMENT`, `CASTFORM_NORMAL`, `DEOXYS_NORMAL`,
`WORMADAM_PLANT`, `CHERRIM_OVERCAST`, `GASTRODON_WEST`,
`GIRATINA_ALTERED`, `SHAYMIN_LAND`, `BASCULIN_RED_STRIPED`,
`BASCULEGION_M`, `DARMANITAN_STANDARD`, `SAWSBUCK_SPRING`,
`TORNADUS_INCARNATE`, `THUNDURUS_INCARNATE`, `LANDORUS_INCARNATE`,
`KELDEO_ORDINARY`, `MELOETTA_ARIA`, `VIVILLON_ICY_SNOW`,
`FLOETTE_RED`, `FLORGES_RED`, `MEOWSTIC_M`, `AEGISLASH_SHIELD`,
`GOURGEIST_AVERAGE`, `XERNEAS_NEUTRAL`, `ZYGARDE_50`,
`HOOPA_CONFINED`, `ORICORIO_BAILE`, `LYCANROC_MIDDAY`,
`WISHIWASHI_SOLO`, `MIMIKYU_DISGUISED`, `TOXTRICITY_AMPED`,
`POLTEAGEIST_PHONY`, `EISCUE_ICE`, `INDEEDEE_M`,
`MORPEKO_FULL_BELLY`, `ZACIAN_HERO`, `ZAMAZENTA_HERO`,
`URSHIFU_SINGLE_STRIKE`, `ENAMORUS_INCARNATE`, `OINKOLOGNE_M`,
`MAUSHOLD_THREE`, `SQUAWKABILLY_GREEN`, `PALAFIN_ZERO`,
`TATSUGIRI_CURLY`, `SINISTCHA_UNREMARKABLE`.
