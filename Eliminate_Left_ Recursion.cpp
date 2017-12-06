#include <iostream>
#include <string>
#include <fstream>
const int Max = 101;
using namespace std;



/*文法类,由产生式、开始符号、终结符号、非终结符号组成*/
typedef class Grammar
{
private:
	/*非终结符存在Vn[i][0]中，Vn[i][2]~Vn[i][9]存放该符的产生式在表达式数组中的位置，
	Vn[i][1]存放位置的结束值*/
	char Vn[Max][10];
	int Vn_num;//非终结符数目

	char Vt[Max];//终结符
	int Vt_num;//终结符数目

	string expressions[Max];//表达式
	int ex_num;//表达式数目

	bool left_wheth;


public:
	Grammar()
	{
		Vn_num = 0;
		Vt_num = 0;
		ex_num = 0;
		left_wheth = false;
	}

	~Grammar(){}

	//判断是否有输入
	bool judge()
	{
		if(Vn_num == 0)
			return false;
		else
			return true;
	}

	//输入函数
	void  input();

	// 输出函数
	void output();

	//在非终结符数组或终结符数组中确定是否有某个文法符号,如果存在，返回它的位置
	//flag=0,搜索非终结符数组；flag=1,搜索终结符数组
	int find_in_array(char x, int flag)
	{
		if(!flag)
		{
			for(int i=0; i<Vn_num; i++)
			{
				if (x == Vn[i][0])
					return i;		
			}
		}
		else
		{
			for(int i=0; i<Vt_num; i++)
				if(x == Vt[i])
					return i;
		}
		return -1;
	}


	// 在string中确定是否有某个字符,如果存在，则返回它的第一个的位置
	int find_in_string(char x, string s)
	{
		for(int i=0; i<s.length(); i++)
			if (x == s[i])
				return i;
		return 0;
	}

	//消除左递归的函数 
	void eliminate();

	//以'|'为边界分割一个表达式
	string* divide(string sr, int n);

	// 以'|'为边界分割一个不完整的表达式
	string* divide_incomplete(string sr, int n);

	// 多个字符串拼接为一个字符串
	void connect(string& sr, string s[], char Vn, int n);

	//统计一个表达式中有多少个分支，以'|'为界限
	int count(string s);

	// 将同一个非终结符为起点的的表达式合并，简化表达式集
	void merge();

	// 代换形如Ai→Ajγ的表达式
	void substitution(int i, int j);

	// 消除直接左递归
	void eliminate_direct(int i);


}Vf;




int main()
{
	Vf G;
	string flag;
	cout<<"文法的输入格式形如A->Aa|a，*代表ε，注意箭头两侧无空格,最后输入#结束!"<<endl;

	while(true)
	{
		cout<<"Do you want to input a grammar?(yes / no):";
		cin>>flag;
		if(flag == "yes")
		{	
			G.input();
			if(G.judge())
			{
				G.eliminate();
				G.output(); 
			}
		}
		else
		{
			if(G.judge())
			{
				cout<<"Thanks for your use!"<<endl;
				break;
			}
		}
	}

	return 0;
}




// 函数实现
void Grammar::input()
{
	cout<<"请输入文法:"<<endl;
	string temp;

	while(true)
	{
		cin>>temp;
		// ifstream f("data.txt");
		// f>>temp;
		if(temp[0] != '#')//当输入不为结束符号时
		 	// while((!f.eof()) && (temp != "#"))
		{
			expressions[ex_num++] = temp;//保存表达式
			int num = find_in_array(temp[0], 0);

			if(num<0)//检验非终结符号为新的时，存入非终结符号集
			{
				//注意
				Vn[Vn_num++][0] = temp[0];
				Vn[Vn_num-1][1] = 2;
				Vn[Vn_num-1][2] = ex_num - 1;
			}
			else//检验非终结符号为旧的时，存储其在表达式数组中的位置
			{
				int a = Vn[num][1];

				Vn[num][a+1] = ex_num - 1;
				Vn[num][1]++;

			}

			for(int i=3; i<temp.length(); i++)//把新的终结符号存入终结符号集
			{
				if(!find_in_array(temp[i], 1) && temp[i]!='|')
					Vt[Vt_num++] = temp[i];
			}
			// f>>temp;

        }
        // for(int xx=0; xx<ex_num; xx++)
        // 	cout<<expressions[xx]<<endl;
        else//输入为结束符号时，退出循环
        	break;
	
	}

}


void Grammar:: output()
{
	if(left_wheth)
	{
		cout<<"消除左递归后的文法："<<endl;
		for(int i=0; i<ex_num; i++)
			cout<<expressions[i]<<endl;
	}
	else
	{
		if(judge())
			cout<<"There is no left recursion."<<endl;
	}
}


void Grammar::eliminate()
{
	int i;
	int j;
	// int sum;
	// string temp;
	int Vn_num_temp = Vn_num;

	merge();//合并具有相同非终结符为起点的表达式
	
	for(i=0; i<Vn_num_temp; i++)
	{	
		for(j=0; j<i; j++)
		{
			substitution(i, j);//代换形如Ai→Ajγ的表达式
		}
		eliminate_direct(i);
	}
}


string* Grammar::divide(string sr, int n)
{
	int i;//sr计数
	int j = 0;//s_divide计数
	char temp;
	// int start = 0, end = 0;//分支的首尾
	string* s_divide = new string[n];

	for(i=3; i<sr.length(); i++)
	{
		temp = sr[i];

		if(temp != '|')
			s_divide[j] += temp;
		else
			j++;
	}

	return s_divide;
	
}


void Grammar::connect(string& sr, string s[], char Vn, int n)
{
	int i;

	sr += Vn;
	sr += "->";

	sr += s[0];
	for(i=1; i<n; i++)
		sr = sr + "|" + s[i];

}


int Grammar::count(string s)
{
	int i;
	int sum=0;

	for(i=0; i<s.length(); i++)
		if(s[i] == '|')
			sum++;

	sum++;
	return sum;
}


void Grammar::merge()
{
	int i, j;
	int index, po, posi;

	for(i=0; i<Vn_num; i++)
	{
		if(Vn[i][1] > 2)
		{
			index = Vn[i][1];
			po = Vn[i][2];

			for(j = 3; j<=index; j++)
			{
				posi = Vn[i][j];
				expressions[po] += ("|" + expressions[posi].substr(3));
				
				for(int x=posi; x<ex_num-1; x++)
				  {
				  	expressions[x] = expressions[x+1];
				  }
				ex_num--;
			}	

			Vn[i][1] = 2;
		}
	}

	for(i=0; i<Vn_num; i++)
	{
		Vn[i][1] = 2;
		Vn[i][2] = i;	
	}

}


void Grammar::substitution(int i, int j)
{
	string temp = expressions[(Vn[i][2])];//该非终结符的表达式
	int sum = count(temp);//该表达式的分支个数
	string* divide_array = divide(temp, sum);//划分该表达式，分支存在divide_array[]中
	
	//对每个分支，检查首字符是否是Aj，即形如Ai→Ajγ的表达式
	//若是，则代换Aj的表达式,得到新的分支divide_array[x]
	for(int x=0; x<sum; x++)
	{
		if(divide_array[x][0] == Vn[j][0])//找到了形如Ai→Ajγ的表达式
		{
			string temp_j = expressions[(Vn[j][2])];//Aj表达式
			int sum_j = count(temp_j);//统计Aj表达式分支数
			string* divide_j = divide(temp_j, sum_j);//分割Aj表达式

			//Aj的分支分别拼接γ，拼接后的分支再连接起来
			for(int y=0; y<sum_j; y++)
				divide_j[y] += divide_array[x].substr(1);
			divide_array[x].clear();
			for(int y=0; y<sum_j; y++)
				divide_array[x] += (divide_j[y] + "|");
			divide_array[x] = divide_array[x].substr(0, divide_array[x].length()-1);
		}
	}

	// 重新拼接
	temp.clear();
	connect(temp, divide_array, Vn[i][0], sum);
	expressions[(Vn[i][2])] = temp;			
}


void Grammar::eliminate_direct(int i)
{
	int index = Vn[i][2];
	int exp_num = count(expressions[index]);

	string e_index;
	string temp;

	string* divide_exp = divide(expressions[index], exp_num);//分割生成式

	//检查分割后的生成式有没有直接左递归，并记录序号
	for(int m=0; m<exp_num; m++)
	{	
		if(divide_exp[m][0] == Vn[i][0])//有直接左递归，就记录序号
		{
			e_index += m;
			left_wheth = true;
		}
		else
		{	
			temp += (divide_exp[m] + '|');//存β1|β2这种
		}

	}
	
	if(temp[temp.length()-1] == '|') 
		temp[temp.length() - 1] = ' ';

	//对直接左递归进行处理
	if(e_index.size() != 0)
	{
		Vn[Vn_num][0] = Vn[i][0];
		Vn[Vn_num][1] = '\'' ;

		//引入的新生成式
		string s;
		s += Vn[Vn_num][0];
		s += Vn[Vn_num][1];
		s += "->";

		Vn_num++;
		ex_num++;

		//处理有左递归的子生成式,即将其添加到引入的生成式中
		//即对每一个A->Ab|c 引入 A'->cA'
		int len = e_index.length();
		int te;
		for(int j=0; j<len; j++)
		{
			te = e_index[j];
			s += divide_exp[te].substr(1);
			s += s[0];
			s += s[1];
			s += '|';
		}
		s += "*";
		expressions[ex_num-1] = s;

		// 对原生成式修改
		// 即把A->Aa|Ab|β1|β2 修改为 A->β1A'|β2A'
		// temp->β1|β2,把temp分割，再组合
		int temp_num = count(temp);
		string* s_new = divide_incomplete(temp, temp_num);
		
		for(int temp_co=0; temp_co<temp_num; temp_co++)
		{
			s_new[temp_co] += s.substr(0, 2);
		}
		
			
		string temp2;
		connect(temp2, s_new, Vn[i][0], temp_num);
		// temp2 += ("|" + temp);
		expressions[index].clear();
		expressions[index] = temp2;

	}

}


string* Grammar::divide_incomplete(string sr, int n)
{
	int i;//sr计数
	int j = 0;//s_divide计数
	char temp;
	// int start = 0, end = 0;//分支的首尾
	string* s_divide = new string[n];

	for(i=0; i<sr.length(); i++)
	{
		temp = sr[i];

		if(temp != '|')
			s_divide[j] += temp;
		else
			j++;
	}

	return s_divide;
}
