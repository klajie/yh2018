// person used in quest

#include <ansi.h>

inherit CLASS_D("generate") + "/chinese";

int  accept_object(object who, object ob);

void random_move();
int  leave();
int  is_stay_in_room()  { return 1; }

void create()
{
        ::create();
        set("gender", random(5) ? "����" : "Ů��");
        NPC_D->generate_cn_name(this_object());
		set("age", 20 + random(40));
        set("long", "");
        set("attitude", "friendly");
        set("chat_chance", 30);
        set("chat_msg", ({ (: random_move :) }));
        set("chat_chance_combat", 120);
        set("scale", 100);
        set("no_get", 1);

        set_temp("apply/armor", 100);
        set_temp("apply/damage", 15);
        set_temp("born_time", time());
        set_temp("dest_time", 1200 + time());

        add_money("silver", 10 + random(20));
        setup();
        if (clonep()) keep_heart_beat();
}
//��������������� by ����
void set_from_me(object me)
{
        int exp;
        mapping my;
		int s, f, x, y, z;

        NPC_D->init_npc_skill(this_object(), NPC_D->get_exp(me));
        my = query_entire_dbase();
		s = this_object()->query_con() + this_object()->query_str();
		f = this_object()->query_int();
		z = (int)this_object()->query_skill("force", 1);
		
		my["max_jingli"] =  my["magic_points"] / 30 + f * 30;
		if (my["animaout"])
            my["max_jingli"] += my["max_jingli"] * 4 / 10;

		my["max_neili"] = z * 15 + my["con"] * 15;
		if (my["breakup"])
            my["max_neili"] += my["max_neili"] * 4 / 10;
		if (my["class"] == "bonze")
			my["max_neili"]  += 500;

        my["max_qi"]     = 100;
		my["max_qi"]    += (my["age"] - 14) * s * 2 / 3;
		my["max_qi"]    += (int)my["max_neili"] / 4;
		 // ̫������ϵ�ѧ�ķ�����
		if ((x = (int)this_object()->query_skill("taoism", 1)) > 39 &&
            (y = (int)this_object()->query_skill("taiji-shengong", 1)) > 39)
            {
                if (x > 350) x = (x - 350) / 2 + 350;
                if (y > 350) y = (y - 350) / 2 + 350;
                if (x > 200) x = (x - 200) / 2 + 200;
                if (y > 200) y = (y - 200) / 2 + 200;

                my["max_qi"] += (x + 100 ) * (y + 100) / 100;
            } else
			// �̲�����ϱ̺�����������
            if ((x = (int)this_object()->query_skill("bihai-chaosheng", 1)) > 39 &&
                (y = (int)this_object()->query_skill("bibo-shengong", 1)) > 39)
                {
                    if (x > 250) x = (x - 250) / 2 + 250;
                    if (y > 250) y = (y - 250) / 2 + 250;
                    if (x > 120) x = (x - 120) / 2 + 120;
                    if (y > 120) y = (y - 120) / 2 + 120;

                    my["max_qi"] += (x + 100 ) * (y + 100) / 100;
                }

            if (my["breakup"])
                my["max_qi"] += my["max_qi"];

		my["eff_qi"]     = my["max_qi"];
        my["qi"]         = my["max_qi"];
		
        my["max_jing"]   = 100;
		my["max_jing"]  += (my["age"] - 14) * s * 2 / 3;
		my["max_jing"]  += (int)my["max_jingli"] / 4;
		if (my["breakup"])
            my["max_jing"] += my["max_jing"];
		if (my["animaout"])
            my["max_jing"] += my["max_jing"];

        my["eff_jing"]   = my["max_jing"];
        my["jing"]       = my["max_jing"];
		my["eff_jingli"]   = my["max_jingli"];
        my["jingli"]       = my["max_jingli"];
        my["neili"]       = my["max_neili"];
        my["quest_count"] = me->query("quest_count");


        if (my["combat_exp"] > 200000 && random(100) < 1)
        {
                // ���� > 200K��5%���ʳ��ֶ������
                set_temp("multi-enemy", 1);
        }

        if (my["combat_exp"] > 800000){
                my["jiali"] = query_skill("force") / 3;
        }
        else{
                my["jiali"] = query_skill("force") / 6;
        }
        if (query("place") == "����")
                set_temp("dest_time", 900 + time());
        else
                set_temp("dest_time", 600 + time());
}

void kill_ob(object ob)
{
        int lvl;

        if (! is_busy())
                exert_function("powerup");

        if (ob->query("quest/id") != query("id"))
        {
                ::kill_ob(ob);
                return;
        }
		
		if (lvl = query_temp("multi-enemy"))
        {
                // ���ֶ������
                delete_temp("multi-enemy");
                call_out("do_help_me", 1 + random(2), ob);
        }

        ::kill_ob(ob);
}
void do_help_me(object ob)
{
        if (! objectp(ob) || environment(ob) != environment() ||
	    ! living(ob))
                return;

        switch (random(3))
        {
        case 0:
                message_vision(HIW "\n$N" HIW "�����ȵ�������һ��" +
                               ob->name(1) +
                               HIW "����λ����Ҫ�ٵ��ˣ��������" +
                               RANK_D->query_self(this_object()) +
                               "һ�ѣ���\n" NOR, this_object(), ob);
                break;

        case 1:
                message_vision(HIW "\n$N" HIW "��Ȼ���വ"
                               "�ڣ������˲���΢΢һ㶡�\n" NOR,
                               this_object());
                break;

        case 2:
                message_vision(HIW "\n$N" HIW "һ����Х������"
                               "�಴������ԶԶ�Ĵ��˿�ȥ��\n" NOR,
                               this_object());
                break;
        }

        call_out("do_continue_help", 1 + random(4), ob);
}

void do_continue_help(object ob)
{
        int n;
        object *obs;

        if (! objectp(ob) || environment(ob) != environment())
                return;

        n = random(3) + 1;
        ob->set_temp("quest/help_count", n);
        message("vision", HIR "˵ʱ�٣���ʱ�죡ͻȻת��" +
                          chinese_number(n) +
                          "���ˣ�һ�����ǰ������������"
                          "�з�����\n" NOR, environment());

        // ���ɰ���
        obs = allocate(n);
        while (n--)
        {
                obs[n] = new(CLASS_D("generate") + "/killed.c");
                NPC_D->set_from_me(obs[n], ob, 100);
                obs[n]->delete_temp("multi-enemy");
                obs[n]->set_temp("help_who", this_object());
                obs[n]->set_temp("is_helper", 1);
        }

        // ����ս�� */
        set_temp("help_ob", obs);
        set_temp("help_count", sizeof(obs));
        obs->move(environment());
        obs->set_leader(this_object());
        obs->kill_ob(ob);
}
int filter_to_bonus(object ob, object aob)
{
        if (! playerp(ob) || ! living(ob) ||
            ! ob->is_killing(query("id")))
                // ��������Ҳ��Ҳ�����ɱ��(NPC)
                return 0;

        // Э���Ķ���һ��
        return (aob == ob->query_temp("quest/assist"));
}

// remove
void remove()
{
        object qob;

        if (objectp(qob = query_temp("quest_ob")))
                // ������Ѿ���ʧ�ˣ�ȡ�������������
                destruct(qob);

        ::remove();
}



// �漴���·���
void random_place(string msg)
{
        object old_env;
        object qob;
        string *not_place;
        string new_place;
        int limit;
        int qcount;

        // ���ԭ�ȴ��������񣬾�ֱ���޸���Ϣ
        if (objectp(qob = query_temp("quest_ob")))
                qob->set("introduce", msg);
        else
                qob = new("/clone/quest/avoid");

        // �漴ѡ������һ���ط�����NPC
        old_env = environment();
        not_place = ({ query("place") });

        if (query("combat_exp") < 500000)
                not_place += ({ "����һ��" });

        if (query("combat_exp") < 800000)
                not_place += ({ "����" });

        NPC_D->place_npc(this_object(), not_place);

        // ���ݸ�ʽ����Ϣ���ɽ��
        msg = replace_string(msg, "$N", name(1));

        qcount = query("quest_count");
        if ((qcount < 100 || qcount >= 900) && random(10) ||
            (qcount >= 100 && qcount < 900) && random(50))
        {
                new_place = query("place");
                if (new_place == "����")
                        limit = 1800;
                else
                        limit = 1200;
        } else
        {
                new_place = "��Զ�ĵط�";
                limit = 1800;

                // ���ٴ����ļ۸�
                set("ask_cheap", 3);
        }
        set_temp("dest_time", limit + time());
        msg = replace_string(msg, "PLACE",  new_place);

        // ��ʼ������¼�������
        qob->set("introduce", msg);
        qob->init_quest(name(1), old_env);
        set_temp("quest_ob", qob);

        return;
}

// �ε���ʱ���л�������
void unconcious()
{
        object *obs;
        object ob;
        mapping my;
        string msg;
        string quester;
        string gender;

        if (! query_temp("is_helper") && ! query_temp("help_ob"))
        {
                  if (random(100) < 8)
                {
                        // 10%��������
                        switch (random(8))
                        {
                        case 0:
                                msg = HIC "$N" HIC "���һ�������¼�"
                                      "����Ѫ���ۿ����Ѳ�֧����Ȼһ"
                                      "����Ӱ�ܳ��������������У���"
                                      "��$n" HIC "��ֻ����ôһ˲�䣬"
                                      "�����Ѿ�����$N" HIC "�����Ӷ�"
                                      "�ߣ���Ȼδ�����ۼ���\n" NOR;
                                break;
                        case 1:
                                msg = HIC "$N" HIC "ҡҡ��׹���ۿ���"
                                      "Ҫ��������Ȼ�������һ����Ѫ"
                                      "��$n" HIC "��æ����һ�ܣ�����"
                                      "��ʱ��$N" HIC "��Ȼ��������"
                                      "����һ�ݣ���ȻƮ��ʮ����֮Զ"
                                      "������Ŀ�ɿڴ���ֻ��$N" HIC
                                      "ɲ�����һ����Ӱ��������ʧ��"
                                      "\n" NOR;
                                break;
                        case 2:
                                msg = HIC "��$n" HIC "һ���֮�£�"
                                      "$N" HIC "ֻ���м�֮�������ﻹ"
                                      "�л���֮�����ۿ���Ҫ��ɥ$n" HIC
                                      "֮�֣�$N" HIC "��Ȼ��ʽһ�䣬"
                                      "�����޷����������Ϻ�����$n" HIC
                                      "��һ�����ѱ�$N" HIC "������"
                                      "�б��ˣ�ֻ��$N" HIC "������"
                                      "���Ӷݶ��ߡ�\n" NOR;
                                break;
                        case 3:
                                msg = HIC "ֻ��$N" HIC "����һ����"
                                      "����ɫ��΢����һЩ����Ȼ�ŷ�"
                                      "�����춯�ص�һ�����ֱ���"
                                      "$n" HIC "��������������һ��"
                                      "��$N" HIC "����һת��ƮȻ��"
                                      "ȥ������$n" HIC "���ֲ�����"
                                      "׷�ϲ��ϣ�\n" NOR;
                                break;
                        case 4:
                                msg = HIC "$n" HIC "�����������ۿ�"
                                      "��Ҫ���֣��������У���$N" HIC
                                      "���Ǳ��޿ɱܣ����޿��ˣ�$N"
                                      HIC "��ɫ��ɫ�ֻʣ����м��͵�"
                                      "ҧ����⣬���ۡ����³���$n"
                                      HIC "ֻ����ǰһ�������������"
                                      "ɢȥ�����ﻹ��$N" HIC "��Ӱ"
                                      "�ӣ�\n" NOR;
                                break;
                        case 5:
                                msg = HIC "$N" HIC "����һ����ȴ��"
                                      "��С�������������У�һ����"
                                      "�������ѵ�����$n" HIC "��״"
                                      "����ǰȥ����Ȼ��ǰ�ǹ�����"
                                      "���ֻ��¼��ˣ�ֻ�����˹���"
                                      "��Ц������$N" HIC "Ī�ţ���"
                                      "��Ҳ����˵������һ���֣���"
                                      "ʮ������ɳ�������$n" HIC
                                      "�ֵ����£����������$N" HIC
                                      "��ȥ�ˡ�\n" NOR;
                                break;
                        case 6:
                                msg = HIC "$N" HIC "�������£���Ȼ"
                                      "������$n" HIC "��ס��ʽ��Ц��"
                                      "����$N" HIC "���㡭��������δ"
                                      "�䣬����$N" HIC "��Ĵ���һ"
                                      "��ֱ��$n" HIC "��Ҫ����$n" HIC
                                      "���һ������æ�мܣ�ֻ������"
                                      "Ϣ���ң�����Ż�£�$N" HIC "Ҳ"
                                      "��׷����ֻ�Ǽ������ߣ�$n" HIC
                                      "���˼�������ȥ����ǰ���ƣ�ȴ"
                                      "�Ѳ���$N" HIC "����Ӱ��\n" NOR;
                                break;
                        default:
                                msg = HIC "$N�������ˣ���Ȼ���У���"
                                      "�߼�ֻ����������һʱ���$n"
                                      HIC "�������ƣ������ٻ���$N"
                                      HIC "��Ȼ�����л��ɳˣ������"
                                      "�Ź����������У�ɱ��һ��Ѫ·"
                                      "���ʹ˲�����\n" NOR;
                                break;
                        }

                        clean_up_enemy();
                        obs = query_enemy();
                        if (sizeof(obs) > 0) ob = obs[0]; else ob = 0;
                        if (! objectp(ob))
                                msg = replace_string(msg, "$n", "����");
                        message_sort("\n" + msg, this_object(), ob);

                        // ���NPC���ж�״̬
                        clear_condition(0);

                        // �ָ�NPC������
                        my = query_entire_dbase();
                        my["eff_jing"] = my["max_jing"];
                        my["jing"]     = my["max_jing"];
                        my["eff_qi"]   = my["max_qi"] * 2 / 3;
                        my["qi"]       = my["eff_qi"];
                        my["neili"]    = my["max_neili"];
                        if (! intp(query_temp("escape_times")))
                                set_temp("escape_times", 1);
                        else
                                add_temp("escape_times", 1);
                        gender = (my["gender"] == "Ů��") ? "��" : "��";
                        // �Ǽ�һ�������Ա���ҿ���׷���м�
                        switch (random(9))
                        {
                        case 0:
                                msg = "��˵$N���˴������ߣ���˵�Ƕ㵽PLACEȥ�ˡ�";
                                break;
                        case 1:
                                msg = "��˵$N���˶��֣���㱻������������"
                                      "���ǽ��������ˣ�����ȥ��PLACE��";
                                break;
                        case 2:
                                msg = "��˵��$N����һ��Ѫս�������ȫ��"
                                      "���ˣ�������ȥ��PLACE�ɡ�";
                                break;
                        case 3:
                                msg = "$N��˵����PLACE��������ǰһ�������������ġ�";
                                break;
                        case 4:
                                msg = "��˵$N���˴����˰ɣ��ף���������˵"
                                      "��PLACEҲ����" + gender + "ѽ��";
                                break;
                        case 5:
                                msg = "������Ҳ��̫�����������˵" + gender + "������PLACE��";
                                break;
                        case 6:
                                msg = "����" + gender + "��ɶ��" + gender + "������PLACEô��";
                                break;
                        case 7:
                                msg = "ǰ����" + gender + "��������������أ�����Ӧ��"
                                      "��ȥ��PLACE�ɡ�";
                        default:
                                msg = "$Nѽ������˵��PLACE������" + gender + "�����������ˡ�";
                                break;
                        }

                        random_place(msg);
                        remove_all_enemy(1);
                        return;
                }
        }

        if (stringp(quester = query_temp("quester")))
        {
                // ����ҵ���ȡ��quest����
                if (objectp(ob = find_player(quester)))
                        ob->set_temp("quest/escape_times", query_temp("escape_times"));
        }
        ::unconcious();
}



// �������NPC������ʱ�򣬼���Ƿ���Խ��н���ĳЩ��ҡ�����
// �Ķ��󣺰�������������������������ˣ� ��Ȼ����Ҫ��ɱ��
// ��������������ɶ�����ξ�������������жԸ�NPC����ɱ
// �������Э�������������ߵ���ҡ�
void die(object killer)
{
        object *obs;            // ���������п��Խ�����
        object dob;             // �������NPC����
        object hob;             // Э����
        object aob;             // ��Э����
        //int qlevel;             // ��������ĵȼ�
        int lvl;                // NPC�ĵȼ�
        int scale;              // NPC���еİ��ֺ����ܵĴ���
        int n;                  // ���Խ������˵���Ŀ
        int exp;                // ��Ҫ�Ϸֵľ���
        int pot;                // ��Ҫ�Ϸֵ�Ǳ��
        int weiwang;            // ��Ҫ�Ϸֵ�����
        int score;              // ��Ҫ�Ϸֵ�����
        int extra_exp;          // ���ε��˵Ķ��⾭��
        int extra_pot;          // ���ε��˵Ķ���Ǳ��
        string quester;         // ��Ҫ��������������ID
        object qob;             // ��Ҫ�������������Ҷ���

        // �ҵ�ɱ����(NPC)���Ǵ����ҵ���
        if (! objectp(dob = killer))
                dob = query_defeated_by();

        if (objectp(dob) && dob->query_temp("owner"))
                dob = dob->query_temp("owner");

        while (1)
        {
                // ��¼���������Ѿ���ֹ
                if (stringp(quester = query_temp("quester")))
                {
                        qob = UPDATE_D->global_find_player(quester);
                        if (qob->query("quest/id") == query("id"))
                                qob->set("quest/notice", "die");
                        UPDATE_D->global_destruct_player(qob, 1);
                }

                if (dob && dob->query("quest/id") == query("id"))
                        // ���Լ�������
                        break;

                if (dob) aob = dob->query_temp("quest/assist");
                if (aob && aob->query("quest/id") == query("id"))
                        // ��Э������ɱ����
                        break;

                // �����������߻�������������Э������ɱ�������¼֮��
                CHANNEL_D->do_channel(this_object(), "rumor",
                                      sprintf("��˵%s����ɱ���ˡ�", name()));
                log_file("static/killed_die", sprintf("%s %s(%s) died at %s because %s.\n",
                                                      log_time(), name(), query("id"),
                                                      environment()->short(),
                                                      dob ? sprintf("%s(%s)", dob->name(1), dob->query("id")) : "none"));

                break;
        }

        if (! aob) aob = dob;

        if (! aob)
        {
                // û���ҵ� QUEST ������
                ::die();
                return;
        }

        // �쿴Ŀǰ����ɱ�ҵ��ˣ��Ƿ�Ҳ��ֵ�ý�����
        obs = all_inventory(environment());
        obs = filter_array(obs, (: filter_to_bonus :), aob);

        if ((n = sizeof(obs)) > 0)
        {
                // ���Ҵ����ҵ��ˣ�����ɱ���ҵ���
                dob = query_defeated_by();

                // ���������أ��ܹ����������е��˷֣����ÿ
                // �˼���һ������������д�������˵���һ�
                // �ö����һЩ������
                lvl = NPC_D->check_level(this_object());
                exp = 15 + random(10) + lvl;
                pot = 9 + random(5) + lvl;
                weiwang = 8 + random(6) + lvl / 2;
                score = 9 + random(6) + lvl / 2;

                // �����������ĵȼ����н�������
                switch (aob->query("quest/level"))
                {
                case 1:
                        break;
                case 2:
                        exp += exp / 2;
                        pot += pot / 2;
                        weiwang += weiwang / 2;
                        score += score / 2;
                        break;
                case 3:
                        exp += exp;
                        pot += pot;
                        weiwang += weiwang;
                        score += score;
                        break;
                default:
                        exp = exp / 2 + 1;
                        pot = pot / 2 + 1;
                        weiwang = weiwang / 2 + 1;
                        score = score / 2 + 1;
                        break;
                }
                // ���NPC�����˰��֣�����Ի�ö��⾭��
                if ((scale = sizeof(query_temp("help_count") +
                     query_temp("escape_times") * 2)) > 0)
                {
                        exp += exp * scale / 2;
                        pot += pot * scale / 2;
                }
                // �����˾��־��飺Ϊʲô����n + 2��������Ϊ
                // �����ҵ��˷����ݾ��飬������n + 1���ټ���
                // ��������������1����ô����n + 2��
                exp = exp / (n + 2) + 1;
                pot = pot / (n + 2) + 1;
                weiwang = weiwang / (n + 2) + 1;
                score = score / (n + 2) + 1;

                // ��������ҵĶ��⽱��
                if (objectp(dob) && member_array(dob, obs) != -1)
                {
                        extra_exp = 1 + random(exp);
                        extra_pot = 1 + random(pot);
                        if (dob->query("combat_exp") < query("combat_exp") / 2)
                        {
                                // ���ֵ����书�Ƚϵͣ���õĶ��⽱����
                                extra_exp *= 2;
                                extra_pot *= 2;
                        } else
                        if (dob->query("combat_exp") > query("combat_exp") * 2)
                        {
                                // ���ֵ����书̫�ߣ���õĶ��⽱����
                                extra_exp /= 2;
                                extra_pot /= 2;
                        }
                }

                // ���н���
                foreach (hob in obs)
                {
                        // �����ӳٽ�����Ϊʲô��ô���ã��ܼ򵥣�
                        // ��Ϊ��ϣ������ڿ���NPC�����Ժ���ܹ�
                        // ����������
                        GIFT_D->delay_bonus(hob, ([ "exp"     : exp + random(4) +
                                                    ((hob == dob) ? extra_exp : 0),
                                                    "pot"     : pot + random(3) +
                                                    ((hob == dob) ? extra_pot : 0),
                                                    "weiwang" : weiwang + random(3),
                                                    "score"   : score + random(3),
                                                    "prompt"  : "����" + name() + "֮��" ]));
                }
        }

        // ��������
        ::die();

        return ;
}

int accept_fight(object ob)
{
        command("say �ã����ǾͱȻ��Ȼ���");
        kill_ob(ob);
        return 1;
}

int accept_hit(object ob)
{
        command("say ����ȥ�ɣ�");
        kill_ob(ob);
        return 1;
}

int accept_kill(object ob)
{
        command("say �ߣ�������");
        return 1;
}

void random_move()
{
        if (query_temp("is_helper"))
        {
                // is a helper
                if (! environment())
                {
                        // not in environment? unavaliable npc.
                        destruct(this_object());
                        return;
                }

                if (! query_temp("help_who"))
                {
                        // owner is killed. destruct this helper
                        if (! living(this_object()))
                                message_vision("$N���������˹�����\n",
                                               this_object());
                        
                        message_vision("$N�������ܣ����Ҵҵ������ˡ�\n",
                                       this_object());
                        destruct(this_object());
                        return;
                }

                // move with owner
                return;
        }

        if (time() > query_temp("dest_time"))
        {
                message_vision("$N����ææ�����ˡ�\n", this_object());
                CHANNEL_D->do_channel(this_object(), "rumor",
                                      sprintf("��˵%s���ӵ�����ɽ���󣬴Ӵ˲��ٳ�û�����䡣", name()));
                log_file("static/killed_die", sprintf("%s %s(%s) vanished because timeout(%d:%d).\n",
                                                      log_time(), name(), query("id"),
                                                      time() - query_temp("born_time"),
                                                      query_temp("dest_time") - query_temp("born_time")));
                destruct(this_object());
                return;
        }
          NPC_D->random_move(this_object());
}