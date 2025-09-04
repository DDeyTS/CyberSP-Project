#ifndef DIALOGUE_SYS_H
#define DIALOGUE_SYS_H
// #define _POSIX_C_SOURCE 200809L

#include <allegro5/allegro.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>

#include <stdbool.h>
#include <stdio.h>
#include <string.h>

typedef enum {
  NPC_JEFFERSON,
  NPC_CLOWNGIRL,
  NPC_RAQUEL,
  NPC_CINDER,
  NPC_CHEEKS,
  NPC_BANGER_THE_KILLER,
  NPC_NICE_CORP,
  NPC_ANAHI,
  NPC_BOZZO,
  NUM_NPCS,
} NPCID;

typedef enum {
  NONE_TOPIC = -1,
  TOPIC_CORP,
  TOPIC_PRICE,
  TOPIC_KINGDOM_OF_CASH,
  TOPIC_RONALDO,
  NUM_TOPICS,
} TopicID;

typedef enum {
  OFF_DLG = 0,
  SHOW_INTRO = 1 << 1,
  DLG_OPEN = 1 << 2,
  CHOOSING_TOPIC = 1 << 3,
} DlgStatsEnum;

typedef struct {
  const char *topic;
  const char *text;
  const char *intro_text; // in case of error, change it for const char.
} Topic;

typedef struct {
  ALLEGRO_BITMAP *portrait_id;
  const char *name;
  int num_topic;
  Topic *topics;
} NPC;

typedef struct {
  char *text;
  float pos_x, pos_y;
} DescriptionObj;

typedef struct {
  int speaker;         // flags the current NPC
  unsigned char flags;
} DlgStats;

void InitDlgBox(ALLEGRO_BITMAP *portrait, const char *name, const char *text);
void DlgExit(void);
NPC *CreateNpc(const char *name, int num_topic);
void FillTopic(NPC *npc, int index, char *topic, const char *text);
void LoadDlg(NPC *npc, const char *topic);
void InitTopicMenu(NPC *npc, int selected);
void InitDescBox(float box_x, float box_y, const char *text);
void InitStdFont(void);
void FillIntro(NPC *npc, const char *text);
void ExplodeFont(void);

extern DlgStats dlgstats;
extern NPC *npc[];
extern DescriptionObj *desc[];
extern ALLEGRO_FONT *font_std, *font_subtitle;
extern ALLEGRO_COLOR font_color, name_color;
extern ALLEGRO_BITMAP *chatbox, *protagonist, *chatbox_light;
extern bool learned_topics[NUM_TOPICS];

#endif
