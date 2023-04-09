#include <iostream>
#include <stdio.h>
#include <WinSock.h>
#include <Windows.h>
#include <mysql.h>
#include <vector>
#include <algorithm>
#include <map>
#include <sqltypes.h> 
#include <atlstr.h>
using namespace std;

//后面统计需要
struct Stu {
	int grade;
	int credit;
	string sno;
	Stu(int gra, int cre, string s) : grade(gra), credit(cre), sno(s) {}
};
char szSno[10], szCno[5], szCname[45], szCpno[5], szSname[25], szSsex[3], szSdept[25], szScholarship[5];
SQLSMALLINT szSage, szGrade, szCcredit;

MYSQL mysql;  //mysql连接
MYSQL_RES* res; //一个结果集结构体   
MYSQL_ROW row; //char** 二维数组，存放一条条记录  
int err_code = 0;                         //保存错误码
char error_info[1024] = {0};               //保存错误信息

const char DataBase_UserName[] = "root";    //数据库用户名username
const char DataBase_Password[] = "wzu202012188";   //数据库密码，填自己的密码
const char DataBase_Host[] = "localhost";  //数据库连接地址
//注意有时候使用主机ip会报错，此时可以将ip改为localhost
const char DataBase_Name[] = "s_t_u202012188";  //database name
unsigned int DataBase_Port = 3306;            //server port

bool ConnectDatabase(); //函数申明
void FreeConnect();   //释放资源
bool ConnectDatabase()
{
	//初始化mysql
	mysql_init(&mysql);   //连接mysql，数据库
	//设置编码方式
	mysql_options(&mysql, MYSQL_SET_CHARSET_NAME, "gbk");
	if (!(mysql_real_connect(&mysql, DataBase_Host, DataBase_UserName, DataBase_Password, DataBase_Name, DataBase_Port, NULL, 0))) //中间分别是主机，用户名，密码，数据库名，端口号（可以写默认0或者3306等），可以先写成参数再传进去
	{
		printf("Error connecting to database:%s\n", mysql_error(&mysql));
		return false;
	}
	else
	{
		MessageBoxA(NULL, "连接MYSQL数据成功！", "消息", MB_OK);
		printf("Connected...\n");
		return true;
	}
}
//释放资源
void FreeConnect()
{
	mysql_close(&mysql);
}

void Menu()
{
	system("cls");
	printf("*********************欢迎使用数据库管理系统*********************\n请输入需要执行的操作：\n");
	printf("1.学生信息维护\n2.课程信息维护\n3.学生成绩维护\n");
	printf("4.学生成绩统计\n5.学生成绩排名\n6.学生学号搜索\n");
	printf("******************************************\n\n");
}


// 新生入学信息增加，学生信息修改
void stuInfo_Preservation()
{
	string Sno, Sname, Ssex, Sage, Sdept, Scholarship;
	while (true)
	{
		char choice;
		system("cls");
		printf("请输入需要的操作 q退出:\n");
		printf("1.新生入学信息增加  2.已有学生信息修改  3.学生信息删除\n");
		rewind(stdin);
		scanf_s("%c", &choice, 1);
		if (choice == 'q' || choice == 'Q')
		{
			printf("学生信息维护结束！\n");
			system("pause");
			break;
		}
		else if (choice == '1')
		{
			printf("请依次输入学生信息Sno Sname Ssex Sage Sdept Scholarship：\n");
			cin >> Sno >> Sname >> Ssex >> Sage >> Sdept >> Scholarship;
			string sql = "insert into Student values('";
			sql += Sno; sql += "','"; sql += Sname; sql += "','"; sql += Ssex; sql += "',";
			sql += Sage; sql += ",'"; sql += Sdept; sql += "','"; sql += Scholarship; sql += "')";

			if (mysql_query(&mysql, sql.c_str()))
			{
				printf("操作失败!\n");
				//查询失败
				err_code = mysql_errno(&mysql);                      //获取错误码
				strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
				printf("%s\n", error_info);
				system("pause");
			}
			else
			{
				printf("操作成功!");
				system("pause");
			}
		}
		else if (choice == '2')
		{
			printf("请输入需要修改的学生的学号\n");
			rewind(stdin);
			cin >> Sno;
			string sql = "delete from Student where Sno = '";
			sql += Sno; sql += '\'';

			if (mysql_query(&mysql, sql.c_str()))
			{
				printf("操作失败!\n");
				//查询失败
				err_code = mysql_errno(&mysql);                      //获取错误码
				strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
				printf("%s\n", error_info);
				system("pause");
			}
			else
			{
				printf("请依次输入修改后的学生信息Sno Sname Ssex Sage Sdept Scholarship：\n");
				rewind(stdin);
				cin >> Sno >> Sname >> Ssex >> Sage >> Sdept >> Scholarship;
				string sql = "insert into Student values('";
				sql += Sno; sql += "','"; sql += Sname; sql += "','"; sql += Ssex; sql += "',";
				sql += Sage; sql += ",'"; sql += Sdept; sql += "','"; sql += Scholarship; sql += "')";

				if (mysql_query(&mysql, sql.c_str()))
				{
					printf("操作失败!\n");
					//查询失败
					err_code = mysql_errno(&mysql);                      //获取错误码
					strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
					printf("%s\n", error_info);
					system("pause");
				}
				else
				{
					printf("操作成功!");
					system("pause");
				}
			}

		}
		else if (choice == '3')
		{
			printf("请输入需要删除的学生的学号\n");
			cin >> Sno;
			string sql = "delete from Student where Sno = '";
			sql += Sno; sql += '\'';
			if (mysql_query(&mysql, sql.c_str()))
			{
				printf("操作失败!\n");
				//查询失败
				err_code = mysql_errno(&mysql);                      //获取错误码
				strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
				printf("%s\n", error_info);
				system("pause");
			}
			else
			{
				printf("操作成功!");
				system("pause");
			}
		}
		else
		{
			printf("输入错误！请重新输入！\n");
			system("pause");
		}
	}
}

// 课程信息维护(增加新课程，修改课程信息，删除没有选课的课程信息)
void courseInfo_Preservation()
{
	string Cno, Cname, Cpno, Ccredit;
	while (true)
	{
		char choice;
		system("cls");
		printf("请输入需要的操作 q退出\n");
		printf("1.增加新课程  2.修改课程信息 3.删除课程信息\n");
		rewind(stdin);
		scanf_s("%c", &choice, 1);
		if (choice == 'q' || choice == 'Q')
		{
			printf("课程信息维护结束！\n");
			system("pause");
			break;
		}
		else if (choice == '1')
		{
			printf("请依次输入课程号Cno, 课程名称Cname, 先修课Cpno, 学分Ccredit：\n");
			rewind(stdin);
			cin >> Cno >> Cname >> Ccredit >> Cpno;
			string sql = "insert into Course values('";
			sql += Cno; sql += "','"; sql += Cname; sql += "','"; sql += Cpno; sql += "',"; sql += Ccredit; sql += ')';

			if (mysql_query(&mysql, sql.c_str()))
			{
				printf("操作失败!\n");
				//查询失败
				err_code = mysql_errno(&mysql);                      //获取错误码
				strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
				printf("%s\n", error_info);
				system("pause");
			}

			else
			{
				printf("操作成功!");
				system("pause");
			}
		}

		else if (choice == '2')
		{
			printf("请输入需要修改的课程号\n");
			rewind(stdin);
			cin >> Cno;
			string sql = "delete from Course where Cno = '";
			sql += Cno; sql += '\'';
			if (mysql_query(&mysql, sql.c_str()))
			{
				printf("操作失败!\n");
				//查询失败
				err_code = mysql_errno(&mysql);                      //获取错误码
				strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
				printf("%s\n", error_info);
				system("pause");
			}
			else
			{
				printf("请依次输入修改后的课程信息 课程号Cno, 课程名称Cname, 先修课Cpno, 学分Ccredit：\n");
				rewind(stdin);
				cin >> Cno >> Cname >> Ccredit >> Cpno;
				string sql = "insert into Course values('";
				sql += Cno; sql += "','"; sql += Cname; sql += "','"; sql += Cpno; sql += "',"; sql += Ccredit; sql += ')';

				if (mysql_query(&mysql, sql.c_str()))
				{
					printf("操作失败!\n");
					//查询失败
					err_code = mysql_errno(&mysql);                      //获取错误码
					strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
					printf("%s\n", error_info);
					system("pause");
				}
				else
				{
					printf("操作成功!");
					system("pause");
				}
			}
		}

		else if (choice == '3') //自动删除所有没有人选的课程
		{
			string sql = "delete from Course where Cno not in (select Cno from SC)";
			if (mysql_query(&mysql, sql.c_str()))
			{
				printf("操作失败!\n");
				//查询失败
				err_code = mysql_errno(&mysql);                      //获取错误码
				strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
				printf("%s\n", error_info);
				system("pause");
			}
			else
			{
				printf("操作成功!");
				system("pause");
			}
		}
		else
		{
			printf("输入错误！请重新输入！\n");
			system("pause");
		}
	}
}

// 录入学生成绩，修改学生成绩
void gradeInfo_Preservation()
{
	string Sno, Cno, Grade;
	while (true)
	{
		char choice;
		system("cls");
		printf("请输入需要的操作 q退出\n");
		printf("1.录入成绩  2.修改成绩\n");
		rewind(stdin);
		scanf_s("%c", &choice, 1);
		if (choice == 'q' || choice == 'Q')
		{
			printf("学生信息维护结束！\n");
			system("pause");
			break;
		}
		else if (choice == '1')
		{
			printf("请依次输入学生学号Sno，考试课程号Cno, 成绩Grade：\n");
			rewind(stdin);
			cin >> Sno >> Cno >> Grade;
			string sql = "insert into SC values('";
			sql += Sno; sql += "','"; sql += Cno; sql += "',"; sql += Grade; sql += ')';
			if (mysql_query(&mysql, sql.c_str()))
			{
				printf("操作失败!\n");
				//查询失败
				err_code = mysql_errno(&mysql);                      //获取错误码
				strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
				printf("%s\n", error_info);
				system("pause");
			}
			else
			{
				printf("操作成功!");
				system("pause");
			}
		}
		else if (choice == '2')
		{
			printf("请依次输入需要修改的学生学号Sno，考试课程号Cno, 成绩Grade：\n");
			rewind(stdin);
			cin >> Sno >> Cno >> Grade;

			string sql = "update SC set Grade = ";
			sql += Grade; sql += " where Sno = '"; sql += Sno; sql += "' and Cno = '"; sql += Cno; sql += '\'';

			if (mysql_query(&mysql, sql.c_str()))
			{
				printf("操作失败!\n");
				//查询失败
				err_code = mysql_errno(&mysql);                      //获取错误码
				strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
				printf("%s\n", error_info);
				system("pause");
			}
			else
			{
				printf("操作成功!");
				system("pause");
			}
		}
		else
		{
			printf("输入错误！请重新输入！\n");
			system("pause");
		}
	}
}


bool cmp(const pair<string, double>& a, const pair<string, double>& b) {
	return a.second > b.second;
}

//按系统计学生的平均成绩、最好成绩、最差成绩、优秀率、不及格人数
void grade_statistic()
{
	string sql;
	int count = 0;
	int totalnum[100] = {0}, excellent[100] = { 0 }, low[100] = { 0 };   //总人数，优秀人数，不及格人数
	double excellent_rate[100];
	SQLINTEGER num = 0; int i = 0; SQLSMALLINT x = 0;

	// 返回各系的考试总人数
	i = 0;
	sql = "select Sdept, count(*) from Student, SC where Student.Sno = SC.Sno group by Sdept";
	if (mysql_query(&mysql, sql.c_str()))
	{
		printf("操作失败!\n");
		//查询失败
		err_code = mysql_errno(&mysql);                      //获取错误码
		strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
		printf("%s\n", error_info);
		system("pause");
	}
	else
	{
		res = mysql_store_result(&mysql);
		while (row = mysql_fetch_row(res))
		{
			x = atoi(string(row[1]).c_str());
			if (!x)
			{
				printf("操作失败!\n");
				//查询失败
				err_code = mysql_errno(&mysql);                      //获取错误码
				strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
				printf("%s\n", error_info);
				system("pause");
			}
			else {
				totalnum[i++] = x;
			}
		}
		mysql_free_result(res);
	}

	count = i;

	// 返回各个系的优秀人数
	i = 0;
	sql = "select Sdept, count(*) from Student, SC where SC.Sno = Student.Sno and Grade > 90 group by Sdept";
	if (mysql_query(&mysql, sql.c_str()))
	{
		printf("操作失败!\n");
		//查询失败
		err_code = mysql_errno(&mysql);                      //获取错误码
		strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
		printf("%s\n", error_info);
		system("pause");
	}
	else
	{
		res = mysql_store_result(&mysql);
		while (row = mysql_fetch_row(res))
		{
			x = atoi(string(row[1]).c_str());
			if (!x)
			{
				printf("操作失败!\n");
				//查询失败
				err_code = mysql_errno(&mysql);                      //获取错误码
				strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
				printf("%s\n", error_info);
				system("pause");
			}
			else {
				excellent[i++] = x;
			}
		}
		mysql_free_result(res);
	}

	// 返回各个系的不及格人数
	i = 0;
	sql = "select Sdept, count(*) from Student, SC where SC.Sno = Student.Sno and Grade < 60 group by Sdept";
	if (mysql_query(&mysql, sql.c_str()))
	{
		printf("操作失败!\n");
		//查询失败
		err_code = mysql_errno(&mysql);                      //获取错误码
		strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
		printf("%s\n", error_info);
		system("pause");
	}
	else
	{
		res = mysql_store_result(&mysql);
		while (row = mysql_fetch_row(res))
		{
			x = atoi(string(row[1]).c_str());
			if (!x)
			{
				printf("操作失败!\n");
				//查询失败
				err_code = mysql_errno(&mysql);                      //获取错误码
				strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
				printf("%s\n", error_info);
				system("pause");
			}
			else {
				low[i++] = x;
			}
		}
		mysql_free_result(res);
	}

	i = 0;
	for (int num = 0; num < count; num++)
	{
		excellent_rate[num] = double(excellent[num]) / totalnum[num];
	}

	SQLSMALLINT max_grade, avg_grade, min_grade;

	//最后输出统计信息
	sql = "select Sdept, Avg(Grade) avg_grade, MAX(Grade) max_grade, MIN(Grade) min_grade from Student, SC where SC.Sno = Student.Sno group by Sdept";
	if (mysql_query(&mysql, sql.c_str()))
	{
		printf("操作失败!\n");
		//查询失败
		err_code = mysql_errno(&mysql);                      //获取错误码
		strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
		printf("%s\n", error_info);
		system("pause");
	}
	else
	{
		//位数一定要比数据库里定义的至少多一位
		i = 0;

		printf("%s %s %s %s %s %s\n", "院系", "平均分", "最高分", "最低分", "优秀率", "不及格人数");
		res = mysql_store_result(&mysql);
		while (row = mysql_fetch_row(res))
		{
			sprintf(szSdept, string(row[0]).c_str());
			avg_grade = atoi(string(row[1]).c_str());
			max_grade = atoi(string(row[2]).c_str());
			min_grade = atoi(string(row[3]).c_str());
			if (!avg_grade)
			{
				printf("操作失败!\n");
				//查询失败
				err_code = mysql_errno(&mysql);                      //获取错误码
				strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
				printf("%s\n", error_info);
				system("pause");
			}
			else
			{
				szSdept[5] = '\0';
				printf("%s   %d     %d     %d     %d%%       %d\n", szSdept, avg_grade, max_grade, min_grade, int(excellent_rate[i] * 100), low[i]);
				i++;
			}
		}
		mysql_free_result(res);
	}
	system("pause");
}

// 按系对学生成绩进行排名，同时显示出学生、课程和成绩信息
void grade_rank()
{
	map<string, vector<Stu> > m;
	string sql = "select Sdept,Student.Sno,Grade,Ccredit from Student, Course, SC where Student.Sno = SC.Sno and SC.Cno = Course.Cno";
	if (mysql_query(&mysql, sql.c_str()))
	{
		printf("操作失败!\n");
		//查询失败
		err_code = mysql_errno(&mysql);                      //获取错误码
		strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
		printf("%s\n", error_info);
		system("pause");
	}
	else
	{
		res = mysql_store_result(&mysql);
		while (row = mysql_fetch_row(res))
		{
			string sno, sdept;
			sprintf(szSdept, string(row[0]).c_str());
			sprintf(szSno, string(row[1]).c_str());
			szGrade = atoi(string(row[2]).c_str());
			szCcredit = atoi(string(row[3]).c_str());
			cout << szSdept << ' ' << szSno << ' ' << szGrade << ' ' << szCcredit << '\n';
			getchar();
			if (!szGrade)
			{
				printf("操作失败!\n");
				//查询失败
				err_code = mysql_errno(&mysql);                      //获取错误码
				strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
				printf("%s\n", error_info);
				system("pause");
			}
			else {
				for (int i = 0; i < 3; i++)
					sdept.push_back(szSdept[i]);
				for (int i = 0; i < 10; i++)
					sno.push_back(szSno[i]);
				m[sdept].push_back(Stu(szGrade, szCcredit, sno));
			}
		}
		mysql_free_result(res);


		//按系统计
		map<string, double> stu_grade;  //加权成绩
		for (auto it = m.begin(); it != m.end(); it++)
		{
			system("cls");
			stu_grade.clear();
			int stu_num = it->second.size();

			int sum = 0, i;
			int class_count = 0;  //一共多少学分
			for (i = 0; i < stu_num; i++)
			{
				if (stu_grade.find(it->second[i].sno) == stu_grade.end() && i != 0)  //出现了新同学，计算之前的，并重新初始化
				{
					stu_grade[it->second[i - 1].sno] = double(sum) / class_count;
					sum = 0;
					class_count = 0;
				}
				class_count += it->second[i].credit;
				sum += it->second[i].credit * it->second[i].grade;
			}
			stu_grade[it->second[i - 1].sno] = double(sum) / class_count;  //最后一个同学单独计算

			vector<pair<string, double>> vec(stu_grade.begin(), stu_grade.end());
			//对线性的vector进行排序
			sort(vec.begin(), vec.end(), cmp);
			printf("开始展示：%s系学生排名\n", it->first.c_str());
			for (int i = 0; i < vec.size(); ++i)
			{
				printf("排名      学号     加权成绩\n");
				printf("Rank%d  %s    %.2f\n", i + 1, vec[i].first.c_str(), vec[i].second);
				printf("成绩如下：\n");
				string sql = "select Sname, Cname, Grade from Student, SC, Course where Student.Sno = ";
				sql += vec[i].first.c_str();
				sql += " and SC.Sno = Student.Sno and Course.Cno = SC.Cno";

				if (mysql_query(&mysql, sql.c_str()))
				{
					printf("操作失败!\n");
					//查询失败
					err_code = mysql_errno(&mysql);                      //获取错误码
					strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
					printf("%s\n", error_info);
					system("pause");
				}
				else
				{
					res = mysql_store_result(&mysql);
					while (row = mysql_fetch_row(res))
					{
						sprintf(szSname, string(row[0]).c_str());
						sprintf(szCname, string(row[1]).c_str());
						szGrade = atoi(string(row[2]).c_str());
						if (!szGrade)
						{
							printf("操作失败!\n");
							//查询失败
							err_code = mysql_errno(&mysql);                      //获取错误码
							strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
							printf("%s\n", error_info);
							system("pause");
						}
						else
						{
							szSname[8] = '\0'; szCname[15] = '\0';
							printf("%s   %s  %d\n", szSname, szCname, szGrade);
						}
					}
					mysql_free_result(res);
					printf("\n");
				}
			}
			printf("\n");
			system("pause");
		}
	}
}

// 输入学号，显示该学生的基本信息和选课信息
void student_search()
{
	string sname, sdept, cname;
	string Sno;
	while (true)
	{
		system("cls");
		printf("请输入需要查询的学生学号Sno 按q退出\n");
		rewind(stdin);
		cin >> Sno;
		if (Sno == "q" || Sno == "Q")
		{
			printf("查询结束！\n");
			system("pause");
			break;
		}
		else
		{
			string sql = "select Student.Sno,Sname,Ssex,Sage,Sdept,Scholarship,SC.Cno,Cname,Grade from Student, Course, SC where Student.Sno = '";
			sql += Sno; sql += "' and Student.Sno = SC.Sno and SC.Cno = Course.Cno";

			if (mysql_query(&mysql, sql.c_str()))
			{
				printf("操作失败!\n");
				//查询失败
				err_code = mysql_errno(&mysql);                      //获取错误码
				strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
				printf("%s\n", error_info);
				system("pause");
			}
			else
			{
				printf("%s         %s   %3s    %s   %s    %s   %s   %s        %s\n", "Sno", "Sname", "Ssex", "Sage", "Sdept", "Scholarship", "Cno", "Cname", "Grade");
				res = mysql_store_result(&mysql);
				while (row = mysql_fetch_row(res))
				{
					sprintf(szSno, string(row[0]).c_str());
					sprintf(szSname, string(row[1]).c_str());
					sprintf(szSsex, string(row[2]).c_str());
					szSage = atoi(string(row[3]).c_str());
					sprintf(szSdept, string(row[4]).c_str());
					sprintf(szScholarship, string(row[5]).c_str());
					sprintf(szCno, string(row[6]).c_str());
					sprintf(szCname, string(row[7]).c_str());
					szGrade = atoi(string(row[8]).c_str());
					if (!szGrade)
					{
						printf("操作失败!\n");
						//查询失败
						err_code = mysql_errno(&mysql);                      //获取错误码
						strncpy(error_info, mysql_error(&mysql), 1024);          //获取错误信息
						printf("%s\n", error_info);
						system("pause");
					}
					else
					{
						szSname[8] = '\0'; szSdept[5] = '\0'; szCname[15] = '\0';
						printf("%-10s  %-5s %-3s     %d     %-5s       %-3s       %-2s  %-10s%d\n", szSno, szSname, szSsex, szSage, szSdept, szScholarship, szCno, szCname, szGrade);
					}
				}
				mysql_free_result(res);
				system("pause");
			}
		}
	}
}

int main()
{
	ConnectDatabase();	//连接数据库
	char choice;
	while (true)
	{
		system("cls");
		Menu();
		printf("请输入需要执行的操作,按q退出！\n");
		rewind(stdin);
		scanf_s("%c", &choice, 1);
		if (choice == 'q' || choice == 'Q')
		{
			printf("再见，欢迎下次使用！\n");
			break;
		}
		else
		{
			switch (choice)
			{
				case '1': 
						stuInfo_Preservation();  
						break;
				case '2': 
						courseInfo_Preservation();  
						break;
				case '3': 
						gradeInfo_Preservation();  
						break;
				case '4': 
						grade_statistic();  
						break;
				case '5': 
						grade_rank();  
						break;
				case '6': 
						student_search();  
						break;
				default: 
						printf("输入错误，请重新输入！\n"); 
						break;
			}
		}
	}
	//3.断开数据库连接
	FreeConnect();
}

