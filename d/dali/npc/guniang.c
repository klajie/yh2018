// guniang.c

inherit NPC;

void create()
{
	set_name("̨�Ĺ���",({ "gu niang", "girl" }) );
        set("gender", "Ů��" );
        set("age", 17);
        set("long", "һλ�����̨�Ĺ����������Ļ��Խ��´���ɫͲȹ��\n");
 	set_temp("apply/defense", 5);
	set("combat_exp", 7000);
//        set("shen", 100);
	set("shen_type", 1);
        set("per", 28);
        set("str", 14);
        set("dex", 15);
        set("con", 16);
        set("int", 15);
        set("attitude", "friendly");
        setup();
        carry_object(__DIR__"obj/tongqun")->wear();
}
