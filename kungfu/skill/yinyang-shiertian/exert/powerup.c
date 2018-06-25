#include <ansi.h>

inherit F_CLEAN_UP;

void remove_effect(object me, int amount);

int exert(object me, object target)
{
        int skill;

        if (target != me)
                return notify_fail("��ֻ��������ʮ�������������Լ���ս������\n");

        if ((int)me->query_temp("powerup"))
                return notify_fail("���Ѿ����˹����ˡ�\n");

        skill = me->query_skill("force");

        message_combatd(HIR "$N" HIR "˫Ŀ΢�գ����ھ�����������������תʮ�����죬��"
                        "ʱֻ��һ�������ޱȵ���������ȫ����\n" NOR, me);

        me->add_temp("apply/attack", skill / 2);
        me->add_temp("apply/defense", skill / 2);
        me->add_temp("apply/unarmed_damage", skill / 2);
        me->add_temp("apply/damage", skill / 2);
        me->add_temp("apply/parry", skill / 2);
        me->add_temp("apply/dodge", skill / 2);
        me->add_temp("apply/armor", skill * 2);
        me->set_temp("powerup", 1);

        me->start_call_out((: call_other, __FILE__, "remove_effect",
                              me, skill / 2 :), skill);

        if (me->is_fighting()) me->start_busy(3);

        return 1;
}

void remove_effect(object me, int amount)
{
        if (me->query_temp("powerup"))
        {
                me->add_temp("apply/attack", -amount);
                me->add_temp("apply/defense", -amount);
                me->add_temp("apply/unarmed_damage", -amount);
                me->add_temp("apply/damage", -amount);
                me->add_temp("apply/parry", -amount);
                me->add_temp("apply/dodge", -amount);
                me->add_temp("apply/armor", -(amount * 4));
                me->delete_temp("powerup");
                tell_object(me, HIW "�������ʮ������������ϣ��������ջص��\n" NOR);
        }
}