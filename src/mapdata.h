/*
    Sylverant Ship Server
    Copyright (C) 2012, 2013, 2015, 2017, 2018 Lawrence Sebald

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU Affero General Public License version 3
    as published by the Free Software Foundation.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Affero General Public License for more details.

    You should have received a copy of the GNU Affero General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#ifndef MAPDATA_H
#define MAPDATA_H

#include <stdint.h>

#include <sylverant/config.h>

#ifdef PACKED
#undef PACKED
#endif

#define PACKED __attribute__((packed))

/* Battle parameter entry (enemy type, basically) used by the server for Blue
   Burst. This is basically the same structure as newserv's BATTLE_PARAM or
   Tethealla's BATTLEPARAM, which makes sense considering that all of us are
   using basically the same data files (directly from the game itself). */
typedef struct bb_battle_param {
    uint16_t atp;
    uint16_t psv;
    uint16_t evp;
    uint16_t hp;
    uint16_t dfp;
    uint16_t ata;
    uint16_t lck;
    uint16_t esp;
    uint8_t unk[12];
    uint32_t exp;
    uint32_t diff;
} PACKED bb_battle_param_t;

/* A single entry in the level table. */
typedef struct level_entry {
    uint8_t atp;
    uint8_t mst;
    uint8_t evp;
    uint8_t hp;
    uint8_t dfp;
    uint8_t ata;
    uint8_t unk[2];
    uint32_t exp;
} PACKED level_entry_t;

typedef level_entry_t bb_level_entry_t;

/* Level-up information table from PlyLevelTbl.prs */
typedef struct bb_level_table {
    struct {
        uint16_t atp;
        uint16_t mst;
        uint16_t evp;
        uint16_t hp;
        uint16_t dfp;
        uint16_t ata;
        uint16_t lck;
    } start_stats[12];
    uint32_t unk[12];
    level_entry_t levels[12][200];
} PACKED bb_level_table_t;

/* PSOv2 level-up information table from PlayerTable.prs */
typedef struct v2_level_table {
    level_entry_t levels[9][200];
} PACKED v2_level_table_t;

/* Enemy data in the map files. This the same as the ENEMY_ENTRY struct from
   newserv. */
typedef struct map_enemy {
    uint32_t base;
    uint16_t reserved0;
    uint16_t num_clones;
    uint32_t reserved[11];
    uint32_t reserved12;
    uint32_t reserved13;
    uint32_t reserved14;
    uint32_t skin;
    uint32_t reserved15;
} PACKED map_enemy_t;

/* Object data in the map object files. */
typedef struct map_object {
    uint32_t skin;
    uint32_t unk1;
    uint32_t unk2;
    uint32_t obj_id;
    float x;
    float y;
    float z;
    uint32_t rpl;
    uint32_t rotation;
    uint32_t unk3;
    uint32_t unk4;
    /* Everything beyond this point depends on the object type. */
    union {
        float sp[6];
        uint32_t dword[6];
    };
} PACKED map_object_t;

/* Enemy data as used in the game. */
typedef struct game_enemy {
    uint32_t bp_entry;
    uint8_t rt_index;
    uint8_t clients_hit;
    uint8_t last_client;
    uint8_t drop_done;
    uint8_t area;
} game_enemy_t;

typedef struct game_enemies {
    uint32_t count;
    game_enemy_t *enemies;
} game_enemies_t;

typedef struct parsed_map {
    uint32_t map_count;
    uint32_t variation_count;
    game_enemies_t *data;
} parsed_map_t;

/* Object data as used in the game. */
typedef struct game_object {
    map_object_t data;
    uint32_t flags;
    uint8_t area;
} game_object_t;

typedef struct game_objects {
    uint32_t count;
    game_object_t *objs;
} game_objs_t;

typedef struct parsed_objects {
    uint32_t map_count;
    uint32_t variation_count;
    game_objs_t *data;
} parsed_objs_t;

#undef PACKED

#ifndef LOBBY_DEFINED
#define LOBBY_DEFINED
struct lobby;
typedef struct lobby lobby_t;
#endif

/* Player levelup data */
extern bb_level_table_t char_stats;
extern v2_level_table_t v2_char_stats;

int bb_read_params(sylverant_ship_t *cfg);
void bb_free_params(void);

int v2_read_params(sylverant_ship_t *cfg);
void v2_free_params(void);

int gc_read_params(sylverant_ship_t *cfg);
void gc_free_params(void);

int bb_load_game_enemies(lobby_t *l);
int v2_load_game_enemies(lobby_t *l);
int gc_load_game_enemies(lobby_t *l);
void free_game_enemies(lobby_t *l);

int map_have_v2_maps(void);
int map_have_gc_maps(void);
int map_have_bb_maps(void);

int load_quest_enemies(lobby_t *l, uint32_t qid, int ver);
int cache_quest_enemies(const char *ofn, const uint8_t *dat, uint32_t sz,
                        int episode);

#endif /* !MAPDATA_H */
