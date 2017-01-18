// stopwords_deleter.cpp: определяет точку входа для консольного приложения.
//

#include "stdafx.h"
#include <fstream>
#include <iostream>
#include <string>
#include "wchar.h"
#include <locale>
#include <codecvt>
#include <vector>


class alphabet
{
	int inc;
	int asize;
	int bom[3];
	std::vector<std::string> word;
	int* wsize;
public:
	alphabet(int num = 165500)
	{
		inc = num;
		asize = 0;
		word.resize(inc);
		bom[0] = 239;
		bom[1] = 187;
		bom[2] = 191;

	}

	void add_word(std::string &str)
	{
		if (this->givePosition(str) == -1)
		{
			if (asize < word.size())
			{
				word[asize] = str;
				asize++;
			}
			else
			{
				word.resize(asize + inc);
				word[asize] = str;
				asize++;
			}
		}
	}

	int givePosition(std::string &str)
	{
		for (int i = 0; i < asize; i++)
			if (word[i] == str)
				return i;

		return -1;

	}

	void readAlphabet(FILE* fin,int & num)
	{
		int iter = 0;

		wsize = new int[num];

		std::string help = "";

		short int h;

		wchar_t ch;
			while (1)
			{
				h = getc(fin);
				if (h == EOF)
					break;
				ch = h;

				if (ch != '\n' && ch!=EOF)
					help += ch;
				else
				{

					word[iter] =help;

					wsize[iter] = help.size();

					help.clear();

					help = "";
					iter++;
					asize++;

					if (iter%inc == 0)
					{
						word.resize(asize + inc);
						
					}
				}
			}
		
	}

	void initialize(FILE* fin)
	{
		short int ch;

		std::string str = "";
		for (int i = 0; i < 3;i++)
		ch = getc(fin);
			while (1)
			{
				str = "";
				ch = getc(fin);
				while (ch != '\t')
					ch = getc(fin);
				ch = getc(fin);
				while (ch != '\t')
					ch = getc(fin);

				while (ch != '\n' && ch != EOF)
				{
					ch = getc(fin);

					while (ch != ' ' && ch != '\t' && ch != '\n'&& ch!=EOF)
					{
						str += ch;
						ch = getc(fin);
					}

					if (str != "")
					{
						this->add_word(str);
						str.clear();
						str = "";
					}


				}

				if (ch == EOF)
					break;
			}


		
	}

	void writeAlphabet(FILE* fout)
	{
		for (int i = 0; i < 3; i++)
			putc(bom[i], fout);
		for (int i = 0; i < asize; i++)
		{
			for (int j = 0; j < word[i].length(); j++)
				putc(word[i][j], fout);
			putc('\n', fout);
		}
	}

	int giveAsize()
	{
		return asize;
	}

	int newgivePosition(std::string &str)
	{

		int help = str.size();

		for (int i = 0; i < asize; i++)
			if (wsize[i]==help)
				if (word[i] == str)
					return i;

		return -1;

	}



};



class vectors
{
	short int** vect;

	int* rank;

	int vsize;

	int msize;

	std::string ranks[10];
public:
	alphabet list;

	vectors(FILE* fin,int col = 1,int num_words = 165480)
	{
		list.readAlphabet(fin,num_words);

		ranks[0] = "science";
		ranks[1] = "style";
		ranks[2] = "culture";
		ranks[3] = "life";
		ranks[4] = "economics";
		ranks[5] = "business";
		ranks[6] = "travel";
		ranks[7] = "forces";
		ranks[8] = "media";
		ranks[9] = "sport";

		vsize = col;

		vect = new short int*[vsize];

		rank = new int[vsize];

		for (int i = 0; i < vsize; i++)
			rank[i] = -2;

		msize = list.giveAsize();

		for (int j = 0; j < vsize; j++)
			vect[j] = new short int[msize];

		for (int i = 0; i < vsize; i++)
			for (int j = 0; j < msize; j++)
				vect[i][j] = 0;


	}

	int whatrank(std::string &str)
	{
		for (int i = 0; i < 10; i++)
			if (ranks[i] == str)
				return i;

		return -1;
	}

	void createFileforRank(FILE* fout,int &ran)
	{
		for (int i = 0; i < vsize; i++)
		{
			if (ran == rank[i])
			{
				putc(1, fout);
				putc(' ', fout);

			}
			else
			{

				putc(-1, fout);
				putc(' ', fout);
			}

			for (int j = 0; j < msize; j++)
			{
				putc(vect[i][j], fout);
				putc(' ', fout);

			}
			putc('\n', fout);
		}
	}

	void createVectorfortrain(FILE* fin)
	{
		std::ofstream fout("ans.txt", std::ios::app);

		char buf[2048];
		fout.rdbuf()->pubsetbuf(buf, sizeof(buf));

		short int ch;
		int iterstr=0;
		std::string str = "";

		int pos = -2;

		for (int i = 0; i < 3; i++)
			ch = getc(fin);
		while (1)
		{
			str = "";
			ch = getc(fin);
			while (ch != '\t')
			{
				str += ch;
				ch = getc(fin);
			}

			rank[iterstr] = whatrank(str);

			str.clear();
			str = "";

			ch = getc(fin);
			while (ch != '\t')
				ch = getc(fin);

			while (ch != '\n' && ch != EOF)
			{
				ch = getc(fin);

				while (ch != ' ' && ch != '\t' && ch != '\n'&& ch != EOF)
				{
					str += ch;
					ch = getc(fin);
				}

				if (str != "")
				{
					pos = this->list.givePosition(str);
					if (pos != -1)
						vect[iterstr][pos] += 1;

					str.clear();
					str = "";
				}


			}

			write_vec(&fout);

			refresh_vec();

			if (ch == EOF)
				break;
		}
	}

	void refresh_vec()
	{
		for (int u = 0; u < msize; u++)
			vect[0][u] = 0;

		rank[0] = -1;
	}

	void write_vec(std::ofstream* fout)
	{
		std::string help;
		help = "";
		//*fout << rank[0] ;
		help += std::to_string(rank[0]);
		for (int u = 0; u < msize; u++)
			if (vect[0][u] != 0)
			{
			help += ' ';
			help += std::to_string(u);
			help += ':';
			help += std::to_string(vect[0][u]);
			}
		*fout << help;
		*fout << '\n';

	}

	void write_vec_test(std::ofstream* fout)
	{
		std::string help;
		help = "";
		//*fout << rank[0] ;
		help += std::to_string(90);
		for (int u = 0; u < msize; u++)
			if (vect[0][u] != 0)
			{
			help += ' ';
			help += std::to_string(u);
			help += ':';
			help += std::to_string(vect[0][u]);
			
			}
		*fout << help;
		*fout << '\n';


	}

	void createVectorforTest(FILE* fin)
	{
		std::ofstream fout("test_vector.txt", std::ios::app);

		char buf[2048];
		fout.rdbuf()->pubsetbuf(buf, sizeof(buf));

		short int ch;
		int iterstr = 0;
		std::string str = "";

		int pos = -2;

		for (int i = 0; i < 3; i++)
			ch = getc(fin);
		while (1)
		{
			
			
			ch = getc(fin);
			while (ch != '\t')
				ch = getc(fin);

			while (ch != '\n' && ch != EOF)
			{
				ch = getc(fin);

				while (ch != ' ' && ch != '\t' && ch != '\n'&& ch != EOF)
				{
					str += ch;
					ch = getc(fin);
				}

				if (str != "")
				{
					pos = this->list.givePosition(str);
					if (pos != -1)
						vect[iterstr][pos] += 1;

					str.clear();
					str = "";
				}


			}

			write_vec_test(&fout);

			refresh_vec();

			if (ch == EOF)
				break;
		}

	}

	

};
void change0to10(FILE* fin, FILE* fout)
{
	int ch;
	int bom[3];
	bom[0] = 239;
	bom[1] = 187;
	bom[2] = 191;

	for (int j = 0; j < 3; j++)
		putc(bom[j], fout);

	while (1)
	{
		


		ch = getc(fin);

		if (ch == '0')
		{
			putc('1', fout);

			putc('0', fout);

		}
		else
		{
			putc(ch, fout);
		}

		while (ch != '\n' && ch != EOF)
		{
			ch = getc(fin);

			putc(ch, fout);
		}

		if (ch == EOF)
			break;

	}

}

void del_pars_err(FILE* fin, FILE* fout)
{
	int ch;

	while (1)
	{



		ch = getc(fin);

		if (ch == '-')
		{
			while (ch != '\n' && ch != EOF)
			{
				ch = getc(fin);

				
			}
		}
		else
		{
			while (ch != '\n' && ch != EOF)
			{
				putc(ch, fout);
				ch = getc(fin);

				
			}
			putc(ch, fout);
		}

		

		if (ch == EOF)
			break;
		
			

	}

}

void giveAnswer(FILE* fin, std::ofstream* fout)
{
	int ch;

	std::string ranks[10];

	ranks[0] = "science";
	ranks[1] = "style";
	ranks[2] = "culture";
	ranks[3] = "life";
	ranks[4] = "economics";
	ranks[5] = "business";
	ranks[6] = "travel";
	ranks[7] = "forces";
	ranks[8] = "media";
	ranks[9] = "sport";

	while (1)
	{



		ch = getc(fin);

		if (ch != '1')
		{
			switch (ch)
			{
				case '2':
				{
					*fout << ranks[2] <<'\n';
					break;

				}

				case '3':
				{
					*fout << ranks[3] << '\n';
					break;

				}

				case '4':
				{
					*fout << ranks[4] << '\n';
					break;

				}
				case '5':
				{
					*fout << ranks[5] << '\n';
					break;

				}
				case '6':
				{
					*fout << ranks[6] << '\n';
					break;

				}
				case '7':
				{
					*fout << ranks[7] << '\n';
					break;

				}
				case '8':
				{
					*fout << ranks[8] << '\n';
					break;

				}
				case '9':
				{
					*fout << ranks[9] << '\n';
					break;

				}


			}
		}
		else
		{
			ch = getc(fin);

			if (ch == '0')
				*fout << ranks[0] << '\n';
			else
			{
				*fout << ranks[1] << '\n';
			}
		}

		while (ch != '\n' && ch != EOF)
		{
			
			ch = getc(fin);


		}

		if (ch == EOF)
			break;



	}
}

int main()
{

	/*
	std::wifstream fin(L"train_text_stem_stwr_nodigit_sym.txt");
	fin.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::consume_header>));

	std::wofstream fout;
	fout.imbue(std::locale(std::locale::empty(), new std::codecvt_utf8<wchar_t, 0x10ffff, std::generate_header>));
	fout.open(L"alphabet.txt");
*/
	//del.delete_stop_words(&fin, &fout);

	//delete_symbol(&fin, &fout);
	
	FILE* fin = fopen("svm_predictions.txt", "r");
	//FILE* fin = fopen("my.txt", "r");
	std::ofstream fout("answer_final.txt");

	//FILE* alph = fopen("alphabet.txt", "r");

	//vectors mu(alph);

	//mu.createVectorfortrain(fin);

	//del_pars_err(fin, fout);

	giveAnswer(fin, &fout);

	fclose(fin);

	

	//fclose(fout);
	//fclose(alph);
	
	system("pause");
	return 0;
}