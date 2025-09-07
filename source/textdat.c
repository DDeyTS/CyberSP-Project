//**************************************************************************
//**
//** File: textdat.c (CyberSP Project)
//** Purpose: Dialogue storage
//**
//** Last Update: 04-09-2025 21:31
//** Author: DDeyTS
//**
//**************************************************************************

#include "textdat.h"
#include "dialoguesys.h"
#include <allegro5/bitmap_io.h>

// PUBLIC FUNCTION PROTOYPES ///////////////////////////////////////////////

void UnlockExtraTopics(void);

// PUBLIC DATA DECLARATIONS ///////////////////////////////////////////////

//==========================================================================
//
//    NpcDlgStorage
//
//    Argument: NPC *npc        - specifies which NPC is talking
//    Return:   void
//
//    NOTE: everytime you'll write a dialogue, take care of the five lines limit
//    otherwise the text won't fit in the upper dialogue box.
//
//==========================================================================

void NpcDlgStorage(NPC *npc[])
{
    npc[NPC_BODYGUARD] = CreateNpc("Bodyguard", 1);
    npc[NPC_BODYGUARD]->portrait_id =
        al_load_bitmap("portraits/bodyguard_face.png");
    FillIntro(
        npc[NPC_BODYGUARD],
        "Tell me what you doing here otherwise I'll kick your ass out of here.");
    FillTopic(
        npc[NPC_BODYGUARD], 0, "Corporation",
        "Tonight I'm working for RefriFesca, a megacorp of ice and soda. Tomorrow "
        "I'll work for a nightclub which'll occur a corporative party. Well... "
        "after that I won't have any job to do. Would ya have one for me? Tell it "
        "for "
        "|Ronaldo.");

    /*
      ID 1, “Clowngirl”.
      Samira, also known as “Clowngirl”, comes from a circus family that
      perfomed in the northeaestern region many years ago. She is funny and
      crazy, and she claiming that a man with a wide hat and inexpressive
      sunglasses is constantly following her. She likes to entertain opharned
      children weekly, while struggling with depression.
    */
    npc[NPC_CLOWNGIRL] = CreateNpc("Clowngirl", 2);
    npc[NPC_CLOWNGIRL]->portrait_id =
        al_load_bitmap("portraits/clowngirl_portrait.png");
    FillIntro(npc[NPC_CLOWNGIRL], "Helloooooooo, guys!");
    FillTopic(npc[NPC_CLOWNGIRL], 0, "Who are you?",
              "Who am I? Do ya really want to ask for that? I'm a clown, dumb!");
    FillTopic(npc[NPC_CLOWNGIRL], 1, "Funny",
              "Yeah, yeah... I'm fuckin' funny. Very, very funny!");

    /*
      ID 2, “Raquel”.
      Raquel is the best call girl (aka doll) in the southern district. She is a
      trans woman who loves sex. Some clients call her nymphomaniac. However,
      she doesn't only work to please depraved people. She's usually paid to
      kill specific targets while she's with them. The opportune kis of death.
      She has cybernetic razors beneath her fake nails.
    */
    npc[NPC_RAQUEL]              = CreateNpc("Raquel", 1);
    npc[NPC_RAQUEL]->portrait_id = al_load_bitmap("portraits/raquel_portrait.png");
    FillIntro(npc[NPC_RAQUEL],
              "Nah-ah, babe. You'll not have me riding on ya. Take a "
              "walk and finds another doll to play with.");
    FillTopic(npc[NPC_RAQUEL], 0, "Doll",
              "They can be all you want to play with, babe. A tall man, a young "
              "woman... After that, you delight of their bodies like never did "
              "before. Of course, for a |price. Always it has a price.");

    /*
      ID 3, “Cinder”.
      Cinder run nearly all the dolls in the metroplex. She is wild and
      audacious, testing the government's limits to take back the control of her
      district that is dominated by privated forces. She also has a big appeal
      for men that has an iron fist to retain what is yours.
    */
    npc[NPC_CINDER]              = CreateNpc("Cinder", 1);
    npc[NPC_CINDER]->portrait_id = al_load_bitmap("portraits/cinder_portrait.png");
    FillIntro(npc[NPC_CINDER],
              "Hi, confuse man. Are you here to stare me like a erect "
              "perv’ or you'll tell something? C'mon. I won't bite you.");
    FillTopic(
        npc[NPC_CINDER], 0, "Bettingshop",
        "That cursed place? The |kingdom |of |cash. I hang out with a |corp "
        "once; he was disgusting, honestly. Anybody who goes there hardly can "
        "keep the bank account filled. If you're wanna step on there, take "
        "care.");

    /*
       ID 4, “Cheeks”.
       Big fan of rock ‘n’ roll, Cheeks is a good vibes singer who throw curses
       at the actual metroplex's government. Crowns of followers see him as a
       choisen one to free everyone of the megacorps' chains.
    */
    npc[NPC_CHEEKS] = CreateNpc("Cheeks", 1);
    npc[NPC_CHEEKS]->portrait_id =
        al_load_bitmap("portraits/bigcheeks_portrait.png");
    FillIntro(npc[NPC_CHEEKS],
              "What's uuuuup, my friend! How can I help ya today?");
    FillTopic(
        npc[NPC_CHEEKS], 0, "Nightclub",
        "There are a lot of over there. Follow your sixth sense, dude, follow "
        "it to the end of the horizon. The sun takes time to be up on the iron "
        "color sky. Iron sky... Wow, it could gimme an awesome song name. "
        "Anyway, I can't help ya now, my friend.");

    /*
      ID 5, “Banger the Killer”.
      COMMENTARY: He is a boss and one of the great villain's goons.
    */
    npc[NPC_BANGER_THE_KILLER] = CreateNpc("Banger the Killer", 2);
    npc[NPC_BANGER_THE_KILLER]->portrait_id =
        al_load_bitmap("portraits/latavelha_portrait.png");
    FillIntro(npc[NPC_BANGER_THE_KILLER], "Prepare to die, mother fucker!");
    FillTopic(npc[NPC_BANGER_THE_KILLER], 0, "Victims",
              "No one could run from me neither from my heavy, chromed arms. "
              "Prepare to cry like a little bitch 'til your blood dryin' out!");
    FillTopic(npc[NPC_BANGER_THE_KILLER], 1, "Boss",
              "Where's my boss? I... I cannot remember. He had only gave me an "
              "order: kill that gunman. For my boss I could do anything! Even "
              "terminating a rat like ya!!! ");
}

//==========================================================================
//
//    UnlockExtraTopics
//
//    Argument: void
//    Return:   void
//
//    NOTE: this function allows the player to unlock new topics (as seen in both
//    GetTopicID and LearnTopic functions) and their respective dialogues.
//
//==========================================================================

void UnlockExtraTopics(void)
{
    // NOTE: each block of code below is so deep because to avoid declaring a lot of
    // pointers at the same time

    {
        NPC *bodyg = npc[NPC_BODYGUARD];
        if (learned_topics[TOPIC_RONALDO]) {
            bodyg->topics =
                realloc(bodyg->topics, sizeof(Topic) * (bodyg->num_topic + 1));
            FillTopic(
                bodyg, bodyg->num_topic, "Ronaldo",
                "He keeps smoking behind of that parking. He's a cool weirdo.");
            bodyg->num_topic++;  // adds early number of topics by 1
        }
    }

    {
        NPC *raquel = npc[NPC_RAQUEL];
        if (learned_topics[TOPIC_PRICE]) {
            raquel->topics =
                realloc(raquel->topics, sizeof(Topic) * (raquel->num_topic + 1));
            FillTopic(raquel, raquel->num_topic, "Price",
                      "Gimme 500 reais and the brunette girl here will give "
                      "you the best "
                      "night o' your life.");
            raquel->num_topic++;
        }
    }
}

//==========================================================================
//
//    DescStorage
//
//    Argument: DescriptionObj *desc[]        - description text index
//    Return:   void
//
//==========================================================================

void DescStorage(DescriptionObj *desc[])
{
    desc[0]        = malloc(sizeof(DescriptionObj));
    desc[0]->text  = strdup("A lone bus stop.");
    desc[0]->pos_x = 325.0f;
    desc[0]->pos_y = 100.0f;

    desc[1]        = malloc(sizeof(DescriptionObj));
    desc[1]->text  = strdup("A shattered window.");
    desc[1]->pos_x = 300.0f;
    desc[1]->pos_y = 100.0f;
}
