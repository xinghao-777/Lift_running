#define max_floor 10
#include<stdio.h>
#include<list>
#include<iostream>
#include<random>
#include <random>
#include<thread>
#include<chrono>
using namespace std;
typedef struct people
{
	int infloor = 0;
	int outfloor = 0;
	int giveuptime = 0;
	int call = (outfloor >= infloor) ? 1 : -1;
}peoples;
list<peoples>  creatpoeple(int num)
{
	list<peoples> outpeople;
	random_device rd; // ���ڻ�ȡ���������������
	mt19937 gen(rd()); // ʹ��Mersenne Twister�㷨����һ�������������

	// ����һ����ָ����Χ�ڵ�������ֲ�
	uniform_int_distribution<> dis1(0, max_floor);
	uniform_int_distribution<> dis2(0, max_floor);
	uniform_int_distribution<> dis3(600, 1200);
	for (int i = 0; i < num; i++)
	{
		// ���������
		int random_num1 = dis1(gen); // ʹ��������������ͷֲ�������һ�������
		int random_num2 = dis2(gen);
		int random_num3 = dis3(gen);
		peoples newpep;
		newpep.infloor = random_num1;
		newpep.outfloor = random_num2;
		newpep.giveuptime = random_num3;
		newpep.call = (newpep.outfloor > newpep.infloor) ? 1 : -1;
		if (newpep.outfloor != newpep.infloor)
		{
			outpeople.push_back(newpep);
		}
		else
		{
			i--;
		}
	}
	return outpeople;
}
typedef struct lift
{
	int floor = 0, is_pepremove = 0, to_close = 0, door_state = 0, state = 0, Direction = 1;
	int move_time = 0, time = 0, keeptime = 0;
	int* flagout = new int;
	int* flagin = new int;
	int othisfloor = 500000;
	int cthisfloor = 500000;
	list<peoples> outpeople;
	list<peoples>people;
	list<peoples>inpeople;
	int direction(int floor, list<peoples> outpeople, list<peoples> inpeople, int direction) //ͨ�����������״̬�������ݵ���һ�����з���
	{
		if (inpeople.size() == 0 && outpeople.size() == 0)
		{
			return direction = 0;
		}
		int state = direction;
		if (direction == 0)
		{
			for (auto it = outpeople.begin(); it != outpeople.end(); it++)
			{
				if (it->infloor == floor)
				{
					return state = 0;
				}
			}
			for (auto it = inpeople.begin(); it != inpeople.end(); it++)
			{
				if (it->outfloor == floor)
				{
					return state = 0;
				}
			}
			for (auto it = inpeople.begin(); it != inpeople.end(); it++)
			{
				if (it->outfloor > floor)
				{
					return state = 1;
				}
			}
			for (auto it = inpeople.begin(); it != inpeople.end(); it++)
			{
				if (it->outfloor < floor)
				{
					return state = -1;
				}
			}
			for (auto it = outpeople.begin(); it != outpeople.end(); it++)
			{
				if (it->infloor > floor)
				{
					return state = 1;
				}
			}
			for (auto it = outpeople.begin(); it != outpeople.end(); it++)
			{
				if (it->infloor < floor)
				{
					return state = -1;
				}
			}
		}
		if (direction == 1)
		{
			for (auto it = inpeople.begin(); it != inpeople.end(); it++)
			{
				if (it->outfloor > floor)
				{
					return state = 1;
				}
			}
			for (auto it = outpeople.begin(); it != outpeople.end(); it++)
			{
				if (it->infloor > floor)
				{
					return state = 1;
				}
			}
			return state = 0;
		}
		if (direction == -1)
		{
			for (auto it = inpeople.begin(); it != inpeople.end(); it++)
			{
				if (it->outfloor < floor)
				{
					return state = -1;
				}
			}
			for (auto it = outpeople.begin(); it != outpeople.end(); it++)
			{
				if (it->infloor < floor)
				{
					return state = -1;
				}
			}
			return state = 0;
		}
	}
	int controler(int floor, list<peoples> outpeople, list<peoples> inpeople, int state)//ͨ����������������״̬Ŀ��ȷ�����ݵ�״̬
	{
		int direction = state;
		state = 0;
		if (inpeople.size() == 0 && outpeople.size() == 0)
		{
			return state = 0;
		}
		if (direction == 1)
		{
			for (auto it = inpeople.begin(); it != inpeople.end(); it++)
			{
				if (it->outfloor == floor)
				{
					return state = 0;
				}

			}
			for (auto it = outpeople.begin(); it != outpeople.end(); it++)
			{
				if (it->call == 1 && it->infloor == floor)
				{
					return state = 0;
				}
			}
			for (auto it = inpeople.begin(); it != inpeople.end(); it++)
			{
				if (it->outfloor > floor)
				{
					return state = 1;
				}
			}
			for (auto it = outpeople.begin(); it != outpeople.end(); it++)
			{
				if (it->infloor > floor)
				{
					return state = 1;
				}
			}
		}
		if (direction == -1)
		{
			for (auto it = inpeople.begin(); it != inpeople.end(); it++)
			{
				if (it->outfloor == floor)
				{
					return state = 0;
				}
			}
			for (auto it = outpeople.begin(); it != outpeople.end(); it++)
			{
				if (it->call == -1 && it->infloor == floor)
				{
					return state = 0;
				}
			}
			for (auto it = inpeople.begin(); it != inpeople.end(); it++)
			{
				if (it->outfloor < floor)
				{
					return state = -1;
				}
			}
			for (auto it = outpeople.begin(); it != outpeople.end(); it++)
			{
				if (it->infloor < floor)
				{
					return state = -1;
				}
			}
		}
		if (direction == 0)
			return 0;
	}//
	void run(int& time, int& Direction, int& state, int& floor, int& move_time, int& keeptime, list<peoples>& outpeople, list<peoples>& inpeople)
	{
		time += 1;
		if (door_state == 0)//���ź�ִ�е�������
		{
			Direction = direction(floor, outpeople, inpeople, Direction);
			state = controler(floor, outpeople, inpeople, Direction);
			move_time += 1;
			if (move_time > 20)
				state = Direction;
			if (move_time >= 30 && state != 0)
			{
				floor += state;
				if (state == 1) {
					cout << endl;
					cout << "����������";
					cout << "[";
					cout << floor + 1;
					cout << "]¥";
				}
				if (state == -1) {
					cout << endl;
					cout << "�����½���";
					cout << "[";
					cout << floor + 1;
					cout << "]¥";
				}
				move_time = 0;
			}
			if (state == 0 && move_time == 20 && (inpeople.size() != 0 || outpeople.size() != 0))//���˾Ϳ���
			{
				door_state = 1;
				if (othisfloor != floor)
				{
					cout << endl;
					cout << "����" << endl;
					othisfloor = floor;
				}
				move_time = 0;
				*flagout = 1;
				*flagin = 1;
			}
			if ((state == 0) && (floor != 0) && (Direction == 0)) //�ϴ�ֹͣ״̬��Ŀǰֹͣ״̬��ʱ���Ϊ30s���ص�һ¥
			{
				keeptime += 1;
				if (keeptime >= 300)
				{
					if (move_time >= 30)
					{
						cout << "����������ʹ�ã�׼���ص�һ¥";
						floor -= 1;
						cout << endl;
						cout << "���ݵ�ǰ¥��";
						cout << "[";
						cout << floor + 1;
						cout << "]";
						cout << endl;
						move_time = 0;
					}
				}

			}
			else
			{
				keeptime = 0;
			}
			if (floor == 0 && inpeople.size() == 0 && outpeople.size() == 0)
			{
				move_time = 0;
			}
		}
		else//����״̬
		{
			move_time += 1;
			if (move_time == 25)//����״̬����ʱ������һ���˽���
			{
				*flagout = 0;
				*flagin = 0;
				for (auto it = inpeople.begin(); it != inpeople.end();)//���������ڲ��ҵ�Ҫ����ȥ���ˣ�����������������
				{
					if (it->outfloor == floor)
					{
						if (move_time == 25)
						{
							cout << "��һ��";
							cout << endl;
							inpeople.erase(it++);
							move_time = 0;
						}
						if (move_time < 25)
						{
							break;
						}
					}
					else
					{
						it++;
					}

				}
				for (auto it = outpeople.begin(); it != outpeople.end();)//������¥��������ҵ�Ҫ����ȥ���ˣ������������ת�Ƶ������ڲ���
				{
					if (it->infloor == floor)
					{
						if (move_time == 25)
						{
							cout << "��һ��";
							cout << endl;
							inpeople.push_back(*it);
							outpeople.erase(it++);
							move_time = 0;
						}
						else
						{
							break;
						}
					}
					else
					{
						it++;
					}
				}
				for (auto it = inpeople.begin(); it != inpeople.end(); it++)//���ҵ��������Ƿ�����Ҫ����
				{
					if (it->outfloor == floor)
					{
						*flagout = 1;
						break;
					}
					else
					{
						*flagout = 0;
						move_time = 0;
					}

				}
				for (auto it = outpeople.begin(); it != outpeople.end(); it++)
				{
					if (it->infloor == floor)
					{
						*flagin = 1;
						break;
					}
					else
					{
						*flagin = 0;
						move_time = 0;
					}

				}

			}
			//���˽�����ִ�й��Ų���
			if (state == 0 && move_time == 20 && *flagout == 0 && *flagin == 0)
			{
				door_state = 0;
				if (floor != cthisfloor)
				{
					cout << "����" << endl;
					cthisfloor = floor;
				}
				move_time = 0;
			}
		};
	}
}Lift;

int main()
{
	Lift lift1;
	int creattime = 50;
	peoples newpep;
	lift1.outpeople = creatpoeple(10);
	for (auto it = lift1.outpeople.begin(); it != lift1.outpeople.end(); it++)
	{
		cout << "��";
		cout << it->infloor + 1;
		cout << " to ";
		cout << it->outfloor + 1;
		cout << "��" << endl;
	}
	cout << endl;
	cout << "���ݵ�ǰ¥��";
	cout << "[1]" << endl;
	while (1)
	{
		lift1.run(lift1.time, lift1.Direction, lift1.state, lift1.floor, lift1.move_time, lift1.keeptime, lift1.outpeople, lift1.inpeople);
		this_thread::sleep_for(chrono::milliseconds(100));
		if (creattime == lift1.time)
		{
			random_device rd; // ���ڻ�ȡ���������������
			mt19937 gen(rd()); // ʹ��Mersenne Twister�㷨����һ�������������
			// ����һ����ָ����Χ�ڵ�������ֲ�
			uniform_int_distribution<> distime(100, 1000);
			uniform_int_distribution<> disnum(0, 1);
			creattime += distime(gen);
			int num = disnum(gen);
			lift1.outpeople.splice(lift1.outpeople.end(), creatpoeple(num));
			cout << endl;
			cout << "Ŀǰ�ⲿ�ȴ���Ա�У�" << endl;
			for (auto it = lift1.outpeople.begin(); it != lift1.outpeople.end(); it++)
			{
				cout << "��";
				cout << it->infloor + 1;
				cout << " to ";
				cout << it->outfloor + 1;
				cout << "�� ";
				cout << endl;
			}
		}

	}
	return 0;
}
