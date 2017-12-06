#include <iostream>
#include <fstream>
#include <string>
using namespace std;

/*用于动态申请数组*/
int nSize = 500;
const int increment = 300;

/*统计变量定义*/
int sentence_num = 0;//语句行数
int words_num = 0;//单词个数
int char_num = 0;//字符个数
int interpunction = 0;//标点个数

/*状态编号*/
int state = 0;

/*关键字数组*/
const int keys_num = 32;

/*注释读取标记*/
bool note_flag = 0;//之前有没有出现未匹配的'/*'
int note_begin;

/*关键字数组*/
string keys[keys_num] = 
{
	"auto", "double", "int", "struct", "break", "else", "long",
	"switch", "case", "enum", "register", "typedef", "char", "extern",
	"return", "union", "const", "float", "short", "unsigned", "contiue",
	"for", "signed", "void", "default", "goto", "sizeof", "volatile",
	"do", "while", "static", "if"
};

/*输出的二元组*/
typedef struct DC
{
	string value;//单词的值
	int code;//类别编码
}word;

/*二元数组*/
word* words = new word[nSize];

/*扩展数组的函数*/
void words_resize();

/*检验单词是否在关键字数组中,如果是，则返回其属性编号*/
int in_keys(string wc);

/*检验字符是不是字母*/
int is_letter(char ch)
{
	if( ((97<=ch) && (ch<=122)) || ((65<=ch) && (ch<=90)) )
		return 1;
	else
		return 0;
};

/*检验字符是不是数字*/
bool is_digit(char ch)
{
	if( (48<=ch) && (ch<=57) )
		return true;
	else
		return false;
};

/*注释间的单词的二元组清空*/
void note_clear();



/*主函数*/
int main()
{
	char ch;
	string sr;
	int len;
	string id;
	fstream f("6班-王硕-测试用例.c");


	
	while(getline(f, sr))
	{
		sentence_num++;//统计行数
		len = sr.length();

		/*先读一行，每行再挨个字符处理*/
		for(int sr_count=0; sr_count<len; sr_count++)
		{
			ch = sr[sr_count];
			char_num++;//统计字符数
			if(words_num > (nSize-3))
				words_resize();

			switch(state)
			{
				/*状态0，根据输入符号选择合适的自动机*/
				case 0:
					id += ch;

					/*如果开头符号是字母或下划线,进入标识符的判断*/
					if((is_letter(ch)) || (ch=='_') )
						state = 1;
					else	
					   {
					   	/*如果开头符号是数字,进入无符号数的判断*/
					   	if(is_digit(ch))
							state = 2;

						else	
						{	
							switch(ch)
							{
								/*如果开头符号是'<'*/
								case '<' :
									state = 8;
									break;

								/*如果开头符号是'>'*/
								case '>' :
									state = 9;
									break;

								/*如果开头符号是'^'或'='或'!'*/
								case ('^') :
									state = 10;
									break;
								case ('=') :
									state = 10;
									break;
								case ('!') :
									state = 10;
									break;

								/*如果开头符号是+*/
								case '+' :
									state = 11;
									break;

								/*如果开头符号是'-'*/
								case '-' :
									state = 12;
									break;

								/*如果开头符号是'&'*/
								case '&' :
									state = 13;
									break;

								/*如果开头符号是'|'*/
								case '|' :
									state = 14;
									break;

								/*如果开头符号是'/'*/
								case '/' :
									state = 15;
									break;

								/*如果开头符号是'*'*/
								case '*' :
									state = 16;
									break;

								/*读到空格，则跳过*/
								case (' ') :
									id.clear();
									break;
								case ('\t') :
									id.clear();
									break;

								/*读到标点符号*/
								case ',':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 71;
									words_num++;
									interpunction++;
									break;
								case ';':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 72;
									words_num++;
									interpunction++;
									break;
								case '\'':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 73;
									words_num++;
									interpunction++;
									sr_count++;
									while(sr[sr_count] != '\'')
										sr_count++;
									words[words_num].value = id;
									words[words_num].code = 73;
									words_num++;
									interpunction++;
									break;
								case '\"':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 75;
									words_num++;
									interpunction++;
									sr_count++;
									while(sr[sr_count] != '\"')
										sr_count++;
									words[words_num].value = id;
									words[words_num].code = 75;
									words_num++;
									interpunction++;
									break;
								case '{':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 77;
									words_num++;
									interpunction++;
									break;
								case '}':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 78;
									words_num++;
									interpunction++;
									break;
								case '(':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 33;
									words_num++;
									interpunction++;
									break;
								case ')':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 34;
									words_num++;
									interpunction++;
									break;
								case '[':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 35;
									words_num++;
									interpunction++;
									break;
								case ']':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 36;
									words_num++;
									interpunction++;
									break;
								case '~':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 39;
									words_num++;
									interpunction++;
									break;
								case '#':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 0;
									words_num++;
									interpunction++;
									break;
								case '.':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 74;
									words_num++;
									interpunction++;
									break;
								case '%':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 46;
									words_num++;
									interpunction++;
									break;
								case ':':
									state = 17;
									words[words_num].value = id;
									words[words_num].code = 76;
									words_num++;
									interpunction++;
									break;



								/*错误处理*/
								default:
									id.clear();
									char_num--;
									state = 18;
									break;
							
							}
						}

					}
					break;
			

				/*状态1，标识符的识别*/
				case 1:
					/*当前字符符合标识符，则存储*/
					if( (is_letter(ch)) || (ch=='_') || (is_digit(ch)) )
						id += ch;
					
					/*当前字符不符合标识符,检验已读的字符串是否是关键字*/
					else
					{	
						int index = in_keys(id);
						// cout<<id<<endl;

						if(index >= 0)//如果是关键字
						{
							words[words_num].value = id;
							words[words_num].code = index;
						}
						else//如果是标识符
						{
							words[words_num].value = id;
							words[words_num].code = 79;
						}

						state = 0;
						sr_count--;//字符指针回退
						char_num--;
						words_num++;//单词计数
						id.clear();//清空存储用的string
					}
					break;


				/*状态2*/
				case 2:
					if(is_digit(ch)) 
						id += ch;
					else
					{	
						switch(ch)
						{
							case '.':
								id += ch;
								state = 3;
								break;

							case 'E':
								id += ch;
								state = 5;
								break;

							default:
								words[words_num].value = id;
								words[words_num].code = 80;
								state = 0;
								sr_count--;//字符指针回退
								char_num--;
								words_num++;//单词计数
								id.clear();//清空存储用的string
								break;
						}
					}
					break;


				/*状态3*/
				case 3:
					id += ch;
					state = 4;
					break;


				/*状态4*/
				case 4:
					if(is_digit(ch))
						id += ch;
					else
						if(ch=='E')
						{
							id += ch;
							state = 5;
						}
							
						else
						{
							words[words_num].value = id;
							words[words_num].code = 80;
							state = 0;
							sr_count--;//字符指针回退
							char_num--;
							words_num++;//单词计数
							id.clear();//清空存储用的string
						}
					break;


				/*状态5*/
				case 5:
					id += ch;
					if((ch == '+') || (ch == '-'))
						state = 6;
					else
						if(is_digit(ch))
							state = 7;
					break;


				/*状态6*/
				case 6:
					id += ch;
					state = 7;
					break;


				/*状态7*/
				case 7:
					if(is_digit(ch))
						id += ch;
					else
					{
						words[words_num].value = id;
						words[words_num].code = 80;
						state = 0;
						sr_count--;//字符指针回退
						char_num--;
						words_num++;//单词计数
						id.clear();//清空存储用的string
					}
					break;


				/*状态8*/
				case 8:
					if(ch == '<')
					{
						id += ch;
						state = 10;
					}
					else
					{
						if(ch == '=')
						{
							id += ch;
							words[words_num].code = 52;
						}
						else
						{
							words[words_num].code = 51;
							sr_count--;//字符指针回退
							char_num--;
						}
						words[words_num].value = id;
						state = 0;
						words_num++;//单词计数
						id.clear();//清空存储用的string
					}					
					break;



				/*状态9*/
				case 9:
					if(ch == '>')
					{
						id += ch;
						state = 10;
					}
					else
					{
						if(ch == '=')
						{
							id += ch;
							words[words_num].code = 53;
						}
						else
						{
							words[words_num].code = 54;
							sr_count--;//字符指针回退
							char_num--;
						}
						words[words_num].value = id;
						state = 0;
						words_num++;//单词计数
						id.clear();//清空存储用的string
					}					
					break;


				/*状态10*/
				case 10:
					if(ch == '=')
					{
						id += ch;
						switch(id[id.length() - 2])
						{
							case '^':
								words[words_num].code = 67;
								break;
							case '=':
								words[words_num].code = 55;
								break;
							case '!':
								words[words_num].code = 56;
								break;
							case '<':
								words[words_num].code = 69;
								break;
							case '>':
								words[words_num].code = 70;
								break;
							default:
								break;
						}
					}
					else
					{
						switch(id[id.length() - 1])
						{
							case '^':
								words[words_num].code = 57;
								break;
							case '=':
								words[words_num].code = 61;
								break;
							case '!':
								words[words_num].code = 38;
								break;
							case '<':
								words[words_num].code = 49;
								break;
							case '>':
								words[words_num].code = 50;
								break;
							default:
								break;
						}
						sr_count--;//字符指针回退
						char_num--;
					}
					words[words_num].value = id;
					state = 0;
					words_num++;//单词计数
					id.clear();//清空存储用的string
					break;


				/*状态11*/
				case 11:
					switch(ch)
					{
						case '-' :
							id += ch;
							words[words_num].code = 42;
							break;
						case '+' :
							id += ch;
							words[words_num].code = 40;
							break;
						case '=' :
							id += ch;
							words[words_num].code = 62;
							break;
						default:
							words[words_num].code = 47;
							sr_count--;//字符指针回退
							char_num--;
							break;
					}
					words[words_num].value = id;
					state = 0;
					words_num++;//单词计数
					id.clear();//清空存储用的string
					break;


				/*状态12*/
				case 12:
					switch(ch)
					{
						case '-' :
							id += ch;
							words[words_num].code = 41;
							break;
						case '>' :
							id += ch;
							words[words_num].code = 37;
							break;
						case '=' :
							id += ch;
							words[words_num].code = 63;
							break;
						default:
							words[words_num].code = 48;
							sr_count--;//字符指针回退
							char_num--;
							break;
					}
					words[words_num].value = id;
					state = 0;
					words_num++;//单词计数
					id.clear();//清空存储用的string
					break;	


				/*状态13*/
				case 13:
					switch(ch)
					{
						case '&' :
							id += ch;
							words[words_num].code = 59;
							break;
						case '=' :
							id += ch;
							words[words_num].code = 66;
							break;
						default:
							words[words_num].code = 44;
							sr_count--;//字符指针回退
							char_num--;
							break;
					}
					words[words_num].value = id;
					state = 0;
					words_num++;//单词计数
					id.clear();//清空存储用的string
					break;	


				/*状态14*/
				case 14:
					switch(ch)
					{
						case '|' :
							id += ch;
							words[words_num].code = 60;
							break;
						case '=' :
							id += ch;
							words[words_num].code = 68;
							break;
						default:
							words[words_num].code = 58;
							sr_count--;//字符指针回退
							char_num--;
							break;
					}
					words[words_num].value = id;
					state = 0;
					words_num++;//单词计数
					id.clear();//清空存储用的string
					break;	


				/*状态15*/
				case 15:
					switch(ch)
					{
						case '=' :
							id += ch;
							words[words_num].code = 65;
							words[words_num].value = id;
							state = 0;
							words_num++;//单词计数
							id.clear();//清空存储用的string
							break;

						/*检测到注释符号'//'*/
						case '/' :
							id += ch;
							words[words_num].code = 83;
							words[words_num].value = id;
							words_num++;//单词计数
							id.clear();//清空存储用的string
							sr_count = len;//略过这一行剩下的
							state = 0;
							break;

						/*检测到注释符号*/
						case '*' :
							id += ch;
							words[words_num].code = 81;
							words[words_num].value = id;
							note_flag = 1;
							state = 0;
							words_num++;//单词计数
							note_begin = words_num;
							id.clear();//清空存储用的string
							break;

						default:
							words[words_num].code = 45;
							sr_count--;//字符指针回退
							char_num--;
							break;
					}
					break;	


				/*状态16*/
				case 16:
					switch(ch)
					{
						/*注释符号，需检验前面有没有与之匹配的注释符号*/
						case '/' :
							if(note_flag)//如果匹配成功，注释间的二元组清空
							{
								id += ch;
								words[words_num].code = 82;
								words[words_num].value = id;
								id.clear();//清空存储用的string
								note_clear();
								words_num++;//单词计数
								state = 0;
							}
							else
							{
								id += ch;
								words[words_num].code = 82;
								words[words_num].value = id;
								state = 0;
								words_num++;//单词计数
								id.clear();//清空存储用的string
							}
							break;

						/*检测到'='*/
						case '=' :
							id += ch;
							words[words_num].code = 64;
							words[words_num].value = id;
							state = 0;
							words_num++;//单词计数
							id.clear();//清空存储用的string
							break;

						default:
							words[words_num].value = "*";
							words[words_num].code = 43;
							sr_count--;//字符指针回退
							char_num--;
							state = 0;
							words_num++;//单词计数
							id.clear();//清空存储用的string
							break;

					}
					break;


				/*状态17，处理标点*/
				case 17:
					sr_count--;
					id.clear();
					state = 0;
					break;


				/*状态18，错误处理*/
				case 18:
					if(!note_flag)
						cout<<"There is wrong in line"<<sentence_num<<"!"<<endl;
					sr_count--;
					char_num--;
					id.clear();
					state = 0;
					break;


				default:break;


			}
			
		}
	
	}
	/*输出*/
	cout<<"The number of lines is "<<sentence_num<<endl;
	cout<<"The number of characters is "<<char_num<<endl;
	cout<<"The number of words is "<<words_num-interpunction<<endl;

	cout<<"The form of output is <words, code>"<<endl;
	for(int i=0; i<words_num; i++)
	{
		cout<<"<";
		cout<<words[i].value;
		cout<<" , ";
		cout<<words[i].code;
		cout<<">"<<endl;
	}
	delete [] words;
}
					


/*扩展数组的函数*/
void words_resize()
{
	nSize += increment;
	word* temp = new word[nSize];

	for(int i=0; i<words_num; i++)
	{
		temp[i].value = words[i].value;
		temp[i].code = words[i].code;
	}

	delete [] words;
	words = temp;
}



/*在关键字数组中查找关键字*/
int in_keys(string wc)
{
	for(int i=0; i<keys_num; i++)
		if(keys[i] == wc)
			return i;
	return -1;
}



/*注释间的单词的二元组清空*/
void note_clear()
{
	for(int i=note_begin; i<words_num; i++)
	{
		words[i].value.clear();
		words[i].code = -1;
	}
	words[note_begin].value = words[words_num].value;
	words[note_begin].code = words[words_num].code;

	words_num = note_begin;
	note_flag = 0;
}

































