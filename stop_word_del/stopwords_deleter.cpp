// stopwords_deleter.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include "wchar.h"
#include <locale>
#include <codecvt>

using namespace std;

class node
{
public:
	node()
	{
		next = NULL;
	}
	wstring  word;
	node* next;

};

class stop_list
{
public:
	node* first;

	stop_list()
	{
		first = NULL;
	}

	void add_word(wstring &str)
	{
		node* help = first;

		if (help != NULL)
		{
			if (help->word == str)
				return;

			while (help->next != NULL)
			{
				if (help->word == str)
					return;
				help = help->next;
			}
			help->next = new node();

			help->next->word = str;

			return;
		}
		else
		{
			help = new node();

			help->word = str;

			first = help;

			return;
		}
	}

	int isstopword(wstring &str)
	{
		node* help = first;
		if (help != NULL)
		{
			if (help->word == str)
				return 1;
			while (help->next != NULL)
			{
				if (help->word == str)
					return 1;

				help = help->next;
			}

		}

		return 0;
	}

	void read_stop_list(wifstream* fin)
	{
		wstring hstr;

		while (!fin->eof())
		{
			(*fin) >> hstr;

			add_word(hstr);
		}
	}

	void write_stop_list()
	{
	
		std::wofstream fout;
		fout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));
		fout.open(L"stop_list_output.txt");

		node* help = first;

		if (fout.is_open())
		{
			while (help != NULL)
			{
				fout << help->word << '\n';

				help = help->next;
			}
		}
	}
};

class list_words
{
public:
	node* first;

	list_words()
	{
		first = NULL;
	}

	void add_word(wstring &str)
	{
		node* help = first;

		if (help != NULL)
		{
			if (help->word == str)
				return;

			while (help->next != NULL)
			{
				if (help->word == str)
					return;
				help = help->next;
			}
			help->next = new node();

			help->next->word = str;

			return;
		}
		else
		{
			help = new node();

			help->word = str;

			first = help;

			return;
		}
	}

	void write_list()
	{

		std::wofstream fout;
		fout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));
		fout.open(L"alphabet.txt");

		node* help = first;

		if (fout.is_open())
		{
			while (help != NULL)
			{
				fout << help->word << '\n';

				help = help->next;
			}
		}
	}
};

class deleter
{
public:
	stop_list list;

	void	delete_stop_words(wifstream* fin, wofstream* fout)
	{

		if (fin->is_open() && fout->is_open())
		{
			std::locale loc("Russian");

			wstring hstr;

			wstring help;

			wchar_t ch;

			wchar_t hch;

			*fin >> hstr;

			fin->get(ch);

			hch = ch;

			if (hstr[0] == L'«')
				hstr[0] = L' ';

			if (hstr[hstr.size() - 1] == L'»')
				hstr[hstr.size() - 1] = L' ';

			if (!list.isstopword(hstr))
				*fout << hstr;

			*fout << ch;

			help = L"";

			while (!fin->eof())
			{
				fin->get(ch);
				ch = tolower(ch, loc);
				if (ch != L' ' && ch != L'\t' && ch != L'\n')
				{
					
					while (ch != L' ' && ch != L'\t' && ch != L'\n')
					{
						help += ch;
						fin->get(ch);
						ch = tolower(ch, loc);
					}
					hstr = help;
					hch = ch;
					help = L"";
					while ((ch == L' ' || ch == L'\t' || ch == L'\n') && !fin->eof())
					{
						hch = ch;
						fin->get(ch);
						ch = tolower(ch, loc);
					}

					help += ch;
				}
				else
				{
					
					hstr = help;
					hch = ch;
					help = L"";
				}
				
				
				
				if (hstr != L"")
				{
					if (hstr[0] == L'«')
						hstr[0] = L' ';

					if (hstr[hstr.size() - 1] == L'»')
						hstr[hstr.size() - 1] = L' ';
				}

				if (!list.isstopword(hstr))
					*fout << hstr;

				*fout << hch;

			}

		}
		else
		{
			cout << "Can't open one of files!" << endl;
		}


	}

};

void delete_digit_and_etc(wifstream* fin,wofstream* fout)
{
	if (fin->is_open() && fout->is_open())
	{
		wchar_t ch;

		std::locale loc("Russian");

		while (!fin->eof())
		{
			fin->get(ch);

			if (!isdigit(ch, loc) && ch != L':' && ch != 8210 && ch != 8211 && ch != 8212 && ch != L'\\' && ch != '/' && ch != L'!' && ch != L'?' && ch != L';' && ch!=L'-')
				*fout << ch;


		}


	}

}

void delete_symbol(std::wifstream* fin, wofstream* fout)
{
	if (fin->is_open() && fout->is_open())
	{
		wchar_t ch;



		std::locale loc("Russian");

		while (!fin->eof())
		{
			fin->get(ch);

			if (!ispunct(ch,loc) && ch!=8210 && ch!=8211 && ch != 8212)
				fout->put(ch);


		}


	}

}


int main()
{
	/*
	//std::wifstream fin(L"C:\\stop_list.txt");
	std::wifstream fin(L"input.txt");
	fin.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

	wstring my;

	//wofstream fout("ex_output.txt");
	std::wofstream fout;
	fout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));
	fout.open(L"ex_output.txt");
	
	wchar_t ch;

	wchar_t* buf = new wchar_t[50];

	int i = 0;


	if (fin.is_open())
	{
		while (!fin.eof())
		{
			
			fin >> my;
			fin.get(ch);
			fout << my;

			fout << ch;

			
			
			//fin >> ch;
			//fout << ch;
			
		}
	}
	*/
	
	std::wifstream finsl(L"stop_list_stem.txt");
	finsl.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));
		
	deleter del;

	del.list.read_stop_list(&finsl);

	char buf1[2048];
	char buf2[2048];

	std::wifstream fin(L"test_text_out.txt");
	fin.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));
	
	fin.rdbuf()->pubsetbuf(buf1, sizeof(buf1));

	std::wofstream fout;
	fout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));
	fout.open(L"test_text_stw.txt");

	fin.rdbuf()->pubsetbuf(buf2, sizeof(buf2));
	del.delete_stop_words(&fin, &fout);

	delete_digit_and_etc(&fin, &fout);

	system("pause");
	return 0;
}

