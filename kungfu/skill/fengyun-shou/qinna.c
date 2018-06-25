// qinna.c ����
 
#include <ansi.h>
#include <combat.h>
 
inherit F_SSERVER;
 
int perform(object me)
{
	string msg;
	object weapon, target;
	int skill, ap, dp, damage;

        if (! target)
        {
	        me->clean_up_enemy();
	        target = me->select_opponent();
        }

	if (! target || ! me->is_fighting(target))
		return notify_fail("�����֡����á�ֻ����ս���жԶ���ʹ�á�\n");

	skill = me->query_skill("fengyun-shou", 1);

	if (skill < 120)
		return notify_fail("��ķ����ֵȼ�����������ʹ�á����á���\n");

	if (me->query("neili") < 100)
		return notify_fail("��������������޷����á����á���\n");
 
	if (me->query_skill_mapped("hand") != "fengyun-shou") 
		return notify_fail("��û�м��������֣��޷�ʹ�á����á���\n");

       if (! living(target))
              return notify_fail("�Է����Ѿ������ˣ��ò�����ô�����ɣ�\n");

	msg = HIC "$N" HIC "����ǰ������$n" HIC "����������ֻ��$P"
	      "���ֺ��ۺ�Ť����ץ��˦�������ۻ����ң�\n" NOR;
 
	ap = me->query_skill("hand");
	dp = target->query_skill("parry");
	if (ap / 2 + random(ap) > dp)
	{
		me->add("neili", -80);
		damage = 40 + ap / 6 + random(ap / 6);
		msg += COMBAT_D->do_damage(me, target, UNARMED_ATTACK, damage, 30,
                                           HIR "$p" HIR "�����ֿ����վ�����$P"
                                           HIR "���������к��Ǳ�ѹ�Ƶ��޷�"
                                           "������\n" NOR);
		me->start_busy(1);
                if (ap / 3 + random(ap) > dp && ! target->is_busy())
                        target->start_busy(ap / 25 + 2);
	} else
	{
		msg += CYN "����$p" CYN "�Ŀ�����$P" CYN
                       "����ʽ�������һһ��⣬û¶���������\n" NOR;
		me->add("neili",-20);
		me->start_busy(3);
	}
	message_combatd(msg, me, target);

	return 1;
}