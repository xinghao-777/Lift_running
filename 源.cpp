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
	random_device rd; // 用于获取真正的随机数种子
	mt19937 gen(rd()); // 使用Mersenne Twister算法创建一个随机数生成器

	// 创建一个在指定范围内的随机数分布
	uniform_int_distribution<> dis1(0, max_floor);
	uniform_int_distribution<> dis2(0, max_floor);
	uniform_int_distribution<> dis3(600, 1200);
	for (int i = 0; i < num; i++)
	{
		// 生成随机数
		int random_num1 = dis1(gen); // 使用随机数生成器和分布来生成一个随机数
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
	int direction(int floor, list<peoples> outpeople, list<peoples> inpeople, int direction) //通过电梯内外的状态决定电梯的下一步运行方向
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
	int controler(int floor, list<peoples> outpeople, list<peoples> inpeople, int state)//通过电梯内外人数和状态目的确定电梯的状态
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
		if (door_state == 0)//关门后执行电梯运行
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
					cout << "电梯上升到";
					cout << "[";
					cout << floor + 1;
					cout << "]楼";
				}
				if (state == -1) {
					cout << endl;
					cout << "电梯下降到";
					cout << "[";
					cout << floor + 1;
					cout << "]楼";
				}
				move_time = 0;
			}
			if (state == 0 && move_time == 20 && (inpeople.size() != 0 || outpeople.size() != 0))//有人就开门
			{
				door_state = 1;
				if (othisfloor != floor)
				{
					cout << endl;
					cout << "开门" << endl;
					othisfloor = floor;
				}
				move_time = 0;
				*flagout = 1;
				*flagin = 1;
			}
			if ((state == 0) && (floor != 0) && (Direction == 0)) //上次停止状态和目前停止状态的时间差为30s，回到一楼
			{
				keeptime += 1;
				if (keeptime >= 300)
				{
					if (move_time >= 30)
					{
						cout << "电梯暂无人使用，准备回到一楼";
						floor -= 1;
						cout << endl;
						cout << "电梯当前楼层";
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
		else//开门状态
		{
			move_time += 1;
			if (move_time == 25)//开门状态下且时间满足一个人进出
			{
				*flagout = 0;
				*flagin = 0;
				for (auto it = inpeople.begin(); it != inpeople.end();)//遍历电梯内部找到要出回去的人，将其从链表中清除；
				{
					if (it->outfloor == floor)
					{
						if (move_time == 25)
						{
							cout << "出一人";
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
				for (auto it = outpeople.begin(); it != outpeople.end();)//遍历本楼层电梯外找到要出回去的人，将其从链表中转移到电梯内部；
				{
					if (it->infloor == floor)
					{
						if (move_time == 25)
						{
							cout << "进一人";
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
				for (auto it = inpeople.begin(); it != inpeople.end(); it++)//查找电梯内外是否还有人要进出
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
			//无人进出就执行关门操作
			if (state == 0 && move_time == 20 && *flagout == 0 && *flagin == 0)
			{
				door_state = 0;
				if (floor != cthisfloor)
				{
					cout << "关门" << endl;
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
		cout << "（";
		cout << it->infloor + 1;
		cout << " to ";
		cout << it->outfloor + 1;
		cout << "）" << endl;
	}
	cout << endl;
	cout << "电梯当前楼层";
	cout << "[1]" << endl;
	while (1)
	{
		lift1.run(lift1.time, lift1.Direction, lift1.state, lift1.floor, lift1.move_time, lift1.keeptime, lift1.outpeople, lift1.inpeople);
		this_thread::sleep_for(chrono::milliseconds(100));
		if (creattime == lift1.time)
		{
			random_device rd; // 用于获取真正的随机数种子
			mt19937 gen(rd()); // 使用Mersenne Twister算法创建一个随机数生成器
			// 创建一个在指定范围内的随机数分布
			uniform_int_distribution<> distime(100, 1000);
			uniform_int_distribution<> disnum(0, 1);
			creattime += distime(gen);
			int num = disnum(gen);
			lift1.outpeople.splice(lift1.outpeople.end(), creatpoeple(num));
			cout << endl;
			cout << "目前外部等待人员有：" << endl;
			for (auto it = lift1.outpeople.begin(); it != lift1.outpeople.end(); it++)
			{
				cout << "（";
				cout << it->infloor + 1;
				cout << " to ";
				cout << it->outfloor + 1;
				cout << "） ";
				cout << endl;
			}
		}

	}
	return 0;
}
