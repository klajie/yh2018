// feilong.c ��ɽ�����������������ɽ���

#include <ansi.h>
#include <combat.h>

#define XIAN "��" HIM "�����������ɽ�" NOR "��"

inherit F_SSERVER;

int perform(object me, object target)
{
        int damage;
        string msg;
        object weapon;
        int ap, dp, fp, pp;
		
		float improve;
		int lvl, m, n;
		string martial;
		string *ks;
		martial = "sword";

        if (userp(me) && ! me->query("can_perform/huashan-jian/xian"))
                return notify_fail("����ʹ�õ��⹦��û�����ֹ��ܡ�\n");

        if (! target) target = offensive_target(me);

        if (! target || ! me->is_fighting(target))
                return notify_fail(XIAN "ֻ����ս���жԶ���ʹ�á�\n");

        if (! objectp(weapon = me->query_temp("weapon")) ||
            (string)weapon->query("skill_type") != "sword")
                return notify_fail("��ʹ�õ��������ԣ�\n");

        if ((int)me->query_skill("huashan-jian", 1) < 160)
                return notify_fail("�㻪ɽ����������죬�޷�ʩչ" XIAN "��\n");

        if ((int)me->query_skill("dodge", 1) < 160)
                return notify_fail("���Ṧ��Ϊ�������޷�ʩչ" XIAN "��\n");

        if ((int)me->query("neili") < 300)
                return notify_fail("�����������������޷�ʩչ" XIAN "��\n");

        if (me->query_skill_mapped("sword") != "huashan-jian")
                return notify_fail("��û�м�����ɽ�������޷�ʹ��" XIAN "��\n");

        if (! living(target))
               return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");
		   
		lvl = to_int(pow(to_float(me->query("combat_exp") * 10), 1.0 / 3)) + 1;
		ks = keys(me->query_skills(martial));
		improve = 0;
		n = 0;
		//������5�����ܵļӳ�
		for (m = 0; m < sizeof(ks); m++)
		{
			if (SKILL_D(ks[m])->valid_enable(martial))
			{
				n += 1;
				improve += (int)me->query_skill(ks[m], 1);
				if (n > 4 )
					break;
			}
		}
		
		improve = improve * 5 / 100 / lvl;

        me->add("neili", -280);

        ap = me->query_skill("sword");
        dp = target->query_skill("dodge");
        fp = target->query_skill("force");
        pp = target->query_skill("parry");
		
		ap += ap * improve;

        message_sort(HIW "\n$N" HIW "��Хһ��������" + weapon->name() + HIW "�漴��ͣת������ӿ��"
                        "�������ǻ�ɽ���ھ�����" HIM "�����������ɽ�" HIW "��������" + weapon->name() + HIW 
                        "����ͻ�䣬һ����ʱ��Ϊ������Ϯ��$n" HIW "����\n" NOR, me, target);        

        msg = HIM "$N" HIM "������ȫ���˵��˽��ϣ�����һ������ͷֱ����\n" NOR;
        if (ap / 2 + random(ap) < dp)
                msg += CYN "$n" CYN "б��������\n" NOR;
        else
        {
                damage = ap + random(ap / 3);
                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 40 + random(10),
                                           HIR "$n" HIR "��æ���ˣ���Ȼ��ܲ�������$N"
                                           HIR "��һ����ÿ�����Ѫ���������ˡ�\n" NOR);
        }

        msg += HIM "\n$N" HIM "Ȧת" + weapon->name() + HIM "���������������ƻֺ룬�����ݺᣬ���˷�����˼��\n" NOR;
        if (ap / 2 + random(ap) < fp)
                msg += CYN "$n" CYN "�����ӽ���Խ����\n" NOR;
        else
        {
                damage = ap + random(ap / 3);
                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 50 + random(10),
                                           HIR "�������Ƶ���켫��$n" HIR "�������ü���"
                                           "���мܣ�ֻ��$N" HIR "����������$n"
                                           HIR "������ʱ��Ѫ���죡\n" NOR);
        }

        msg += HIM "\n$N" HIM "�������ã�����$p" HIM "���ģ�����֮�죬��λ֮׼����������δ�š�\n" NOR;
        if (ap / 2 + random(ap) < pp)
                msg += CYN "$n" CYN "���ڿ��У��������У�ֻ�ܻӳ�һ�У�������$N"
                       CYN "���ϣ���һ������ƮȻ��ȥ��\n" NOR;
        else
        {
                damage = ap + random(ap / 2);
                msg += COMBAT_D->do_damage(me, target, WEAPON_ATTACK, damage, 60 + random(10),
                                           HIR "$n" HIR "���ڿ��У��������ü����У�ֻ��$N"
                                           HIR "�˽��ӹ���$n" HIR "�����Һ�����Ѫ���·ɽ���\n" NOR);
        }

        me->start_busy(3);
        message_combatd(msg, me, target);
        return 1;
}