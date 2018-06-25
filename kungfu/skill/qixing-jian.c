inherit SKILL;

mapping *action = ({
([      "action":"$Nʹһʽ���ǳ����졹������$w�������Һ�ɨ����$n��$l",
        "force" : 70,
        "attack": 10,
        "dodge" : 10,
        "damage": 13,
        "lvl" : 0,
        "skill_name" : "�ǳ�����",
        "damage_type":  "����"
]),
([      "action":"$N̤��һ���������Ǿ��¡�������$w���������һ����������$n��$l",
        "force" : 74,
        "attack": 15,
        "dodge" : 20,
        "damage": 19,
        "lvl" : 10,
        "skill_name" : "���Ǿ���",
        "damage_type":  "����"
]),
([      "action":"$N����$wһ����һ�С�ƫ�����ǡ���ббһ�������ó�������$n��$l",
        "force" : 85,
        "attack": 20,
        "dodge" : 25,
        "damage": 21,
        "lvl" : 20,
        "skill_name" : "ƫ������",
        "damage_type":  "����"
]),
([      "action":"$N����$w����������Ȧ��һʽ���ǳڵ�������һ���������$n��$l",
        "force" : 118,
        "attack": 25,
        "dodge" : 30,
        "damage": 28,
        "lvl" : 30,
        "skill_name" : "�ǳڵ���",
        "damage_type":  "����"
]),
([      "action":"$Nһʽ�����Ʊ�������������չ������$w����$n��$l",
        "force" : 130,
        "attack": 30,
        "dodge" : 35,
        "damage": 32,
        "lvl" : 40,
        "skill_name" : "���Ʊ���",
        "damage_type":  "����"
]),
([      "action":"$N����$wбָ���죬һʽ�����ǻ�ۡ�����׼$n��$lбб����",
        "force" : 150,
        "attack": 33,
        "dodge" : 45,
        "damage": 39,
        "lvl" : 50,
        "skill_name" : "���ǻ��",
        "damage_type":  "����"
]),
([      "action":"$Nһʽ�����������$w��ն�������缲�������$n���ؿ�",
        "force" : 193,
        "attack": 40,
        "dodge" : 55,
        "damage": 54,
        "lvl" : 60,
        "skill_name" : "�������",
        "damage_type":  "����"
]),
([      "action":"$Nʹһʽ����ת���ǡ�������$w��������$n���ؿڣ������ɵ�",
        "force" : 220,
        "attack": 45,
        "dodge" : 55,
        "damage": 63,
        "lvl" : 80,
        "skill_name" : "��ת����",
        "damage_type":  "����"
])
});

int valid_enable(string usage)
{
	return usage == "sword" || usage == "parry";
}

int valid_learn(object me)
{
        if (me->query("max_neili") < 100)
                return notify_fail("���������Ϊ̫��޷�ѧϰ���ǽ�����\n");

        if (me->query_skill("sword", 1) < 10)
                return notify_fail("��Ļ���������Ϊ�������޷�ѧϰ���ǽ�����\n");

        if (me->query_skill("sword", 1) < me->query_skill("qixing-jian", 1))
                return notify_fail("��Ļ�������ˮƽ���ޣ��޷�������������ǽ�����\n");

        return 1;
}

string query_skill_name(int level)
{
        int i;
        for(i = sizeof(action)-1; i >= 0; i--)
                if(level >= action[i]["lvl"])
                        return action[i]["skill_name"];
}

mapping query_action(object me, object weapon)
{
        int i, level;
        level = (int) me->query_skill("qixing-jian",1);
        for(i = sizeof(action); i > 0; i--)
                if(level > action[i-1]["lvl"])
                        return action[NewRandom(i, 20, level/5)];
}

int practice_skill(object me)
{
        object weapon;

        if (! objectp(weapon = me->query_temp("weapon")) ||
            (string)weapon->query("skill_type") != "sword")
                return notify_fail("��ʹ�õ��������ԡ�\n");

        if ((int)me->query("qi") < 40)
                return notify_fail("����������������ǽ�����\n");

        if ((int)me->query("neili") < 40)
                return notify_fail("����������������ǽ�����\n");

        me->receive_damage("qi", 35);
        me->add("neili", -33);
        return 1;
}

string perform_action_file(string action)
{
        return __DIR__"qixing-jian/" + action;
}