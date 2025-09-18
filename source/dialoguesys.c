//**************************************************************************
//**
//** File: dialoguesys.c (CyberSP Project)
//** Purpose: Any text to display happens here
//**
//** Last Update: 17-09-2025 15:09
//** Author: DDeyTS
//**
//**************************************************************************

#include "dialoguesys.h"
#include "bitmap.h"
#include "game.h"
#include "main.h"
#include "textdat.h"

// MACROS ///////////////////////////////////////////////////////////////////

#define WORDS_MAX 1024
#define DESCRIPTIONS_MAX 100

// EXTERNAL FUNCTION PROTOTYPES /////////////////////////////////////////////

void    InitStdFont();
TopicID GetTopicID(const char *topic);
void    LearnTopic(const char *topic);
void    ExplodeFont();

// EXTERNAL DATA DECLARATIONS ///////////////////////////////////////////////

NPC             *npc[NUM_NPCS];
DescriptionObj **desc = NULL;  // TODO: finding a lighter way to quantify the
                               // amount of description texts inside this array
int obj_desc = 0;

unsigned int desc_count = 0;
DlgStats     dlgstats   = {0, SHOW_INTRO};

ALLEGRO_FONT *font_std, *font_subtitle;
ALLEGRO_COLOR font_color, name_color;
bool          learned_topics[NUM_TOPICS] = {false};

// PRIVATE DATA DEFINITIONS /////////////////////////////////////////////////

static ALLEGRO_COLOR highlight_color;
static ALLEGRO_FONT *font_name;

//==========================================================================
//
//    CreateNpc
//
//    Argument: const char *name    - NPC's name
//              int num_topic       - number of topics the NPC can have
//    Return:   NPC                 - pointer to allocated NPC struct
//
//==========================================================================

NPC *CreateNpc(const char *name, int num_topic)
{
    NPC *npc       = malloc(sizeof(NPC));  // allocates NPC ID
    npc->name      = name;
    npc->num_topic = num_topic;
    npc->topics    = malloc(sizeof(Topic) * num_topic);
    return npc;
}

//==========================================================================
//
//    FillTopic
//
//    Argument: NPC *npc            - pointer to the specific NPC
//              int index           - index of the topic to fill
//              int char *topic     - topic tag name
//              int char *text      - respective dialogue
//    Return:   void
//
//==========================================================================

void FillTopic(NPC *npc, int index, char *topic, const char *text)
{
    npc->topics[index].topic = strdup(topic);
    npc->topics[index].text  = strdup(text);
}

//==========================================================================
//
//    FillIntro
//
//    Argument: NPC *npc        - pointer to the specific NPC
//              char * text     - introduction dialogue
//    Retunr:   void
//
//==========================================================================

void FillIntro(NPC *npc, const char *text)
{
    npc->topics->intro_text = strdup(text);
}

//==========================================================================
//
//    InitDlgBox
//
//    Argument: ALLEGRO_BITMAP * portrait    - portrait of NPC
//              const char *name             - NPC's name
//              char *text                   - dialogue text to display
//    Return:   void
//
//==========================================================================

void InitDlgBox(ALLEGRO_BITMAP *portrait, const char *name, const char *text)
{
    // Box attributes
    float box_w = 640;
    float box_h = 300;
    float x = 0, y = 0;

    float portrait_size = 173;

    // Text attributes
    float padding     = 30;
    float text_x      = x + portrait_size + 2 * padding;
    float line_y      = y + padding / 2;
    float text_max_w  = (x + box_w) - text_x - padding + 30;
    float text_max_h  = (y + box_h) - line_y - padding + 60;
    float safe_width  = text_max_w - 10.0f;
    float safe_height = text_max_h - 10.0f;

    //
    // Render Faces
    //

    if (portrait) {
        al_draw_scaled_bitmap(portrait, 0, 0, al_get_bitmap_width(portrait),
                              al_get_bitmap_height(portrait), x + padding,
                              y + padding - 12, portrait_size, portrait_size, 0);
        // }
        // else if (!portrait) {
        //     perror("Fail to load portrait!\n");
        //     exit(1);
    }
    else if (!portrait)
        portrait = DBG_portrait;

    al_draw_scaled_bitmap(protagonist, 0, 0, al_get_bitmap_width(protagonist),
                          al_get_bitmap_height(protagonist), 450, 290,
                          portrait_size, portrait_size, 0);

    //
    // Render Dialogue Box
    //

    al_draw_bitmap(chatbox, 0, 0, 0);

    // NOTE: below there's that little green light in the right side
    {
        static int    light_frame  = 0;
        static double last_frame   = 0;
        const double  frame_delay  = 0.2;
        const int     total_frames = 8;
        const int     frame_w = 8, frame_h = 18;

        // controls the animation loop of the green light
        double elapsed_time = al_get_time() - last_frame;
        for (; elapsed_time >= frame_delay; elapsed_time -= frame_delay) {
            light_frame++;  // adds a frame
            // if it reaches its limit, reset it
            if (light_frame >= total_frames) {
                light_frame = 0;
            }
            // animation reset to the beginning
            last_frame += frame_delay;
        }

        if (chatbox_light) {
            al_draw_bitmap_region(chatbox_light, 0, light_frame * frame_h, frame_w,
                                  frame_h, 632, 21, 0);
        }
    }

    if (name) {
        al_draw_text(font_name, name_color, x + portrait_size + 2 * padding - 15,
                     y + padding / 2, 0, name);
    }

    //
    // Text Loader
    //

    int line_height = al_get_font_line_height(font_std);
    int max_lines   = safe_height / line_height;
    int line_count  = 0;

    char buffer[WORDS_MAX];
    strncpy(buffer, text, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    char *word            = strtok(buffer, " ");  // put spaces between each word
    char  line[WORDS_MAX] = "";                   // stores full line
    float cursor_x        = text_x;               // initial position to write

    do {
        bool        is_highlight = false;
        const char *draw_word    = word;

        if (word[0] == '|') {
            is_highlight = true;
            draw_word    = word + 1;  // ignores '|'
            LearnTopic(draw_word);
        }

        //
        // Line Break
        //

        int word_width  = al_get_text_width(font_std, draw_word);
        int space_width = al_get_text_width(font_std, " ");

        if (cursor_x + word_width > text_x + safe_width) {
            line_y += line_height;
            cursor_x = text_x;
            line_count++;

            if (line_count >= max_lines) break;
        }

        //
        // Print Dialogue
        //

        al_draw_text(font_std, is_highlight ? highlight_color : font_color,
                     cursor_x - 15, line_y + 20, 0, draw_word);

        cursor_x += word_width + space_width;
        // it updates cursor to draw the current
        // word. The next word will come in the
        // sequence with a space between it.

        word = strtok(NULL, " ");
    } while (word != NULL && line_count < max_lines);

    //
    // Print Last Line
    //

    if (line_count < max_lines && strlen(line) > 0) {
        al_draw_text(font_std, font_color, text_x, line_y + 20, 0, line);
    }
}

//==========================================================================
//
//    DlgExit
//
//    Argument: void
//    Return:   void
//
//==========================================================================

void DlgExit(void)
{
    dlgstats.flags &= ~(DLG_OPEN | SHOW_INTRO);
    active_topic = -1;  // turns unable to choose topics
}

//==========================================================================
//
//    LoadDlg
//
//    Argument: NPC *npc            - pointer to the specific NPC
//              const char *topic   - dialogue to load
//    Return:   void
//
//==========================================================================

void LoadDlg(NPC *npc, const char *dialogue)
{
    //
    // Introduction
    //

    if (npc->topics->intro_text) {
        InitDlgBox(npc->portrait_id, npc->name, (char *)npc->topics->intro_text);
    }

    //
    // Topic
    //

    for (int i = 0; i < npc->num_topic; i++) {
        if (strcmp(npc->topics[i].topic, dialogue) == 0) {
            InitDlgBox(npc->portrait_id, npc->name, (char *)npc->topics[i].text);

            if (!npc->portrait_id) {
                printf("Warning: NPC '%s' is without portrait\n", npc->name);
                exit(1);
            }

            return;
        }
    }
}

//==========================================================================
//
//    InitTopicMenu
//
//    Argument: NPC *npc        - pointer to the specific NPC
//              int selected    - index of the currently selected topic
//    Return    void
//
//    FIXME: need to control the height of the topic list.
//
//==========================================================================

void InitTopicMenu(NPC *npc, int selected)
{
    if (npc->num_topic <= 0) return;

    float         x = 100, y = 350;
    ALLEGRO_COLOR color;

    // upper title
    al_draw_text(font_subtitle, al_map_rgb(255, 255, 255), x, y - 15, 0,
                 "Ask About...");

    //
    // Topic List
    //

    int topic_per_col = 4;
    int spacing_x     = 120;
    int spacing_y     = 20;

    for (int t = 0; t < npc->num_topic; t++) {
        int col = t / topic_per_col;
        int row = t % topic_per_col;

        float draw_x = x + col * spacing_x;
        float draw_y = y + row * spacing_y;

        color =
            (t == selected) ? al_map_rgb(255, 255, 0) : al_map_rgb(255, 255, 255);
        al_draw_textf(font_std, color, draw_x, draw_y, 0, "%s",
                      npc->topics[t].topic);
    }
}

//==========================================================================
//
//    GetTopicID
//
//    Argument: const char* topic       - specific topic to get
//    Return:   TopicID                 - enum for the topic
//
//==========================================================================

TopicID GetTopicID(const char *topic)
{
    if (strcmp(topic, "corp") == 0) return T_CORP;
    if (strcmp(topic, "price.") == 0) return T_PRICE;
    if (strcmp(topic, "kingdom") == 0) return T_KINGDOM_OF_CASH;
    if (strcmp(topic, "Ronaldo.") == 0) return T_RONALDO;
    if (strcmp(topic, "nightclub") == 0) return T_NIGHTCLUB;

    return NONE_TOPIC;
}

//==========================================================================
//
//    LearnTopic
//
//    Argument: const char *topic       - specific topic to learn
//    Return:   void
//
//==========================================================================

void LearnTopic(const char *topic)
{
    TopicID id = GetTopicID(topic);
    if (id != NONE_TOPIC && !learned_topics[id]) {
        learned_topics[id] = true;

        // NOTE: debugger below
        printf("Novo tópico aprendido: %s (ID %d)\n", topic, id);

        UnlockExtraTopics();
    }
}

//==========================================================================
//
//    InitDescBox
//
//    Argument: float box_x            - X position of the description box
//              float box_y            - Y position of the description box
//              const char *text       - description text to display
//    Return:   void
//
//==========================================================================

void InitDescBox(float box_x, float box_y, const char *text)
{
    const float padding    = 10.0f;
    const float wrap_w_cap = 200.0f;  // max width to line break
    const int   line_h     = al_get_font_line_height(font_std);
    const int   space_w    = al_get_text_width(font_std, " ");

    char buffer[WORDS_MAX];
    strncpy(buffer, text, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    float line_w = 0.0f;
    float widest = 0.0f;  // bigger width between all the lines
    int   lines  = 1;     // starts from 1st line

    // as long as each word in the buffer, split by spaces and stored in the
    // pointer, isn't null: the pointer will update to store the next word
    for (char *w = strtok(buffer, " "); w; w = strtok(NULL, " ")) {
        const char *draw      = (w[0] == '|') ? w + 1 : w;
        int         current_w = al_get_text_width(font_std, draw);

        // adds padding before printed words (except at first line)
        int add = (line_w > 0) ? space_w : 0;
        // if a word + space will blow up the width limit:
        if (line_w + add + current_w > wrap_w_cap) {
            if (line_w > widest) widest = line_w;  // stores bigger line so far
            lines++;
            line_w = (float)current_w;
        }
        else {
            // continue accumulating words in the same line
            line_w += add + current_w;
        }
    }
    if (line_w > widest) widest = line_w;

    //
    // Description Window (with padding around the text)
    //

    float inner_w = widest;
    float box_w   = inner_w + 2 * padding;
    // minimum width
    if (box_w < 60.0f) box_w = 60.0f;
    // maximum width
    if (box_w > wrap_w_cap + 2 * padding) box_w = wrap_w_cap + 2 * padding;
    float box_h = lines * line_h + 2 * padding;

    //
    // Draws Description Box
    //

    al_draw_filled_rectangle(box_x, box_y, box_x + box_w, box_y + box_h,
                             al_map_rgba(0, 0, 50, 100));

    //
    // Print Text
    //

    float text_x = box_x + padding;
    float text_y = box_y + padding;
    float safe_w = box_w - 2 * padding;  // maximum width inside the box

    // resets word buffer to use strtok() again
    strncpy(buffer, text, sizeof(buffer));
    buffer[sizeof(buffer) - 1] = '\0';

    float cx = 0.0f;  // horizontal cursor inside the current line

    for (char *w = strtok(buffer, " "); w; w = strtok(NULL, " ")) {
        bool        is_highlight = (w[0] == '|');
        const char *draw         = is_highlight ? w + 1 : w;
        int         ww           = al_get_text_width(font_std, draw);

        int add = (cx > 0.0f) ? space_w : 0;

        // if it print words beyond the safe width:
        if (cx + add + ww > safe_w) {
            text_y += line_h;  // new line
            cx  = 0.0f;        // reset cursor
            add = 0;           // first word doesn't have space before it
        }
        // last position of this word
        cx += add;

        al_draw_text(font_std, is_highlight ? highlight_color : font_color,
                     text_x + cx, text_y, 0, draw);

        // cursor move towards the next word
        cx += ww;
    }
}

void AddDescription(DescriptionObj ***desc_ptr, unsigned int *count,
                    const char *text, float x, float y)
{
    *desc_ptr = realloc(*desc_ptr, sizeof(DescriptionObj *) * (*count + 1));
    (*desc_ptr)[*count]        = malloc(sizeof(DescriptionObj));
    (*desc_ptr)[*count]->text  = strdup(text);
    (*desc_ptr)[*count]->pos_x = x;
    (*desc_ptr)[*count]->pos_y = y;
    (*count)++;
}

//==========================================================================
//
//    InitStdFont
//
//    Argument: void
//    Return:   void
//
//    NOTE: this function loads all the fonts of the game.
//
//==========================================================================

void InitStdFont()
{
    //
    // Error Catcher
    //

    const char *path = "fonts/Steelflight.ttf";
    FILE       *f    = fopen(path, "r");
    if (!f) {
        perror("Error from fopen");
        fprintf(stderr, "Font doesn't found: %s\n", path);
        exit(1);
    }
    fclose(f);

    //
    // Loading Fonts
    //

    int f_size = 13;

    font_std = al_load_ttf_font("fonts/Steelflight.ttf", f_size, 0);
    if (!font_std) {
        printf("Error: fail to load font_std!\n");
        exit(1);
    }

    font_name = al_load_ttf_font("fonts/Steelflight.ttf", f_size + 1, 0);

    font_subtitle = al_load_ttf_font("fonts/Steelflight.ttf", f_size + 2, 0);

    font_color      = al_map_rgb(255, 255, 255);  // white
    name_color      = al_map_rgb(255, 255, 0);    // yellow
    highlight_color = al_map_rgb(255, 215, 0);    // golden yellow
}

//==========================================================================
//
//    ExplodeFont
//
//    Argument: void
//    Return:   void
//
//    NOTE: this function destroys every font loaded before.
//
//==========================================================================

void ExplodeFont()
{
    al_destroy_font(font_std);
    al_destroy_font(font_name);
}

//==========================================================================
//
//    DescBoxIsOpen
//
//    Argument: bool show_desc        - checks if description box is open
//    Return:   bool
//
//==========================================================================

bool DescBoxIsOpen(bool show_desc)
{
    if (!show_desc) return false;

    InitDescBox(desc[obj_desc]->pos_x, desc[obj_desc]->pos_y,
                desc[obj_desc]->text);

    return true;
}

//==========================================================================
//
//    DlgBoxIsOpen
//
//    Argument: bool open_desc        - checks if dialogue box is open
//    Return:   bool
//
//==========================================================================

bool DlgBoxIsOpen(bool open_dlg)
{
    if (!open_dlg) return false;

    bool show_intro = (dlgstats.flags & SHOW_INTRO) == SHOW_INTRO;

    if (open_dlg) {
        if (show_intro) {
            InitDlgBox(npc[dlgstats.speaker]->portrait_id,
                       npc[dlgstats.speaker]->name,
                       npc[dlgstats.speaker]->topics->intro_text);
        }
        else if (active_topic >= 0) {
            const char *topic =
                npc[dlgstats.speaker]->topics[selected_topic].topic;
            LoadDlg(npc[dlgstats.speaker], topic);
        }
        InitTopicMenu(npc[dlgstats.speaker], selected_topic);
    }

    return true;
}

//==========================================================================
//
//    InitTextBoxes
//
//    Argument: void
//    Return:   bool
//
//==========================================================================

bool InitTextBoxes(void)
{
    bool dlg_open = (dlgstats.flags & DLG_OPEN) == DLG_OPEN;

    bool descbox = DescBoxIsOpen(show_desc);
    bool dlgbox  = DlgBoxIsOpen(dlg_open);

    return descbox || dlgbox;
}
