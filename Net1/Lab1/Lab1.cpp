// Lab1.cpp: определяет точку входа для консольного приложения.
// Lab1.cpp: defining the entry point for the console application.
//
//1.Single-layer neural network
//Boolean function:
//(x1+x2)x3+x4
//

#include "stdafx.h"
#include "stdio.h"
#include "iostream"
#include "locale.h"

using namespace std;

struct BinaryEnters
{
	bool x[4];
};

struct SynapWeights
{
	float w[5];
};

bool SetFuncValue(BinaryEnters values)//Entering one set of values
{
	bool F = (values.x[3] + values.x[2])*values.x[1] + values.x[0];
	return F;
};

bool CheckBoolFuncValue(bool BoolFuncValue[16], bool OriginalBoolFuncValue[16])
{
	//A comparison of the values of the function with original
	bool check = 0;
	for (int i = 0; i < 16; i++)
	{
		if (BoolFuncValue[i] != OriginalBoolFuncValue[i])
			return check;
	}
	return 1;
};

float F(float net, int FA)
{
	float out = 0;
	switch (FA)
	{
	case 1:
		out = net + 0.5;
		break;
	case 2:
		out = 0.5*(net / (1 + abs(net)) + 1);
		break;
	case 3:
		out = 1 / (1 + exp(-net));
		break;
	case 4:
		out = 0.5*(tanh(net) + 1);
		break;
	default: cout << "Ошибка при расчете функции активации.";
	}

	return out;
};

float dF(int FA, float net)
{
	float derivative=0;

	switch (FA)
	{
	case 1:
		derivative = 1;
		break;
	case 2:
		derivative = 1 / (2*pow(1 + abs(net),2));
		break;
	case 3:
		derivative = exp(net)/(exp(net)+1);
		break;
	case 4:
		derivative = 2/pow(exp(net)+exp(-net),2);
		break;
	default: cout << "Ошибка при расчете производной функции активации.";
	}

	return derivative;
};
int _tmain(int argc, _TCHAR* argv[])
{
	setlocale(LC_ALL, "Russian");
	bool OriginalBoolFuncValue[16]; //True values of the function on this set
	BinaryEnters BinEnters[16]; //Value sets of the function

	//Setting values and printing the corresponding values of the function
	for (char i = 0; i < 16; i++)
	{
		for (char j = 3; j >= 0; j--)
		{
			cout << (BinEnters[i].x[j] = (i >> j) & 1);
		}
		cout << " " << (OriginalBoolFuncValue[i] = SetFuncValue(BinEnters[i])) << endl;
	}


	//Synaptic weights:
	SynapWeights SynWeights[2];
	//SynWeights[0]-weights of the current iteration
	//SynWeights[1]-weights of the next iteration

	//Setting values of weights
	for (int i = 0; i <= 4; i++)
	{
		SynWeights[0].w[i] = 0;
		SynWeights[1].w[i] = 0;
	}

	//Output signal
	bool Y[16];
	float net = 0;				//Сетевой вход
								//Net input
	const float Tetta = 0.25;	//Норма обучения
								//The rate of learning
	int E = 0;					//Суммарная квадратичная ошибка
								//The total squared error
	float out = 0;				//Сетевой выход
								//Net output
	float Sygma = 0;			//Ошибка
								//Error
	float DeltaW = 0;
	float derivative = 0;		//Производная ФА
	bool test = 0;

	cout << "Выберите пороговую функцию:" << endl;
	int FA = 0;
	cout << "1.Пороговая" << endl;
	cout << "2.Модуль" << endl;
	cout << "3.Сигмоидальная" << endl;
	cout << "4.Гиперболическая" << endl;

	do{
		cin >> FA;
	} while ((FA < 1) && (FA > 4));

	switch (FA)
	{
	case 1:
		cout << "Выбрана пороговая функция." << endl;
		break;
	case 2:
		cout << "Выбрана функция модуля." << endl;
		break;

	case 3:
		cout << "Выбрана сигмоидальня функция." << endl;
		break;

	case 4:
		cout << "Выбрана гиперболическая функция." << endl;
		break;
	default: cout << "Ошибка в выборе функции активации.";
	}

	//1 case
	//Using all combinations of variables x1,x2,x3,x4 
	//
	cout << endl << "Выберите вариант обучения:" << endl;
	cout << "1.С использованием всех комбинаций переменных x1,x2,x3,x4" << endl;
	cout << "2.Используем часть переменных для обучения НС, а другую часть для её тестирования" << endl;
	int variant = 0;
	do{
		cin >> variant;
	} while ((variant != 1) && (variant != 2));
	int inc = 0; //Max count of eras

	if (variant == 1)
	{
		do
		{
			E = 0;
			for (int k = 0; k < 16; k++)
			{
				net = 0;
				for (int i = 0; i <= 3; i++)
				{
					net = net + SynWeights[0].w[i] * BinEnters[k].x[i];
				}
				net += SynWeights[0].w[4];
				out = F(net, FA);
				if (out >= 0.5) Y[k] = 1; else Y[k] = 0;
				Sygma = OriginalBoolFuncValue[k] - Y[k];
				if (Sygma != 0)
				{
					for (int i = 0; i <= 3; i++)
					{
						derivative = dF(FA, net);
						DeltaW = Tetta*Sygma*derivative*BinEnters[k].x[i];
						SynWeights[1].w[i] = SynWeights[0].w[i] + DeltaW;
						SynWeights[0].w[i] = SynWeights[1].w[i];
					}
					//Recalculating the weight of the displacement neuron
					DeltaW = Tetta*Sygma*derivative;
					SynWeights[1].w[4] = SynWeights[0].w[4] + DeltaW;
					SynWeights[0].w[4] = SynWeights[1].w[4];
					E += abs(Sygma);
				}
			}
			cout << "Y=(";
			for (int k = 0; k < 16; k++)
			{
				cout << Y[k] << ",";
			}
			cout << ")" << " W=(";

			for (int k = 3; k >= 0; k--)
			{
				cout << SynWeights[0].w[k] << ", ";
			}
			cout << SynWeights[0].w[4];

			cout << ")" << "  E=" << E << "  inc=" << (inc + 1) << endl;
			inc++;
		} while ((E != 0) && (inc != 100));
	}
	else
	{
		//2 Случай
		//Using one part of variables for teaching neuronet and the second one for testing 
		//
		do
		{
			E = 0;
			for (int k = 0; k <= 10; k++)
			{
				net = 0;
				for (int i = 0; i <= 3; i++)
				{
					net = net + SynWeights[0].w[i] * BinEnters[k].x[i];
				}
				net += SynWeights[0].w[4];
				out = F(net, FA);
				if (out >= 0.5) Y[k] = 1; else Y[k] = 0;
				Sygma = OriginalBoolFuncValue[k] - Y[k];
				if (Sygma != 0)
				{
					for (int i = 0; i <= 3; i++)
					{
						derivative = dF(FA, net);
						DeltaW = Tetta*Sygma*derivative*BinEnters[k].x[i];
						SynWeights[1].w[i] = SynWeights[0].w[i] + DeltaW;
						SynWeights[0].w[i] = SynWeights[1].w[i];
					}
					//Recalculating the weight of the displacement neuron
					DeltaW = Tetta*Sygma*derivative;
					SynWeights[1].w[4] = SynWeights[0].w[4] + DeltaW;
					SynWeights[0].w[4] = SynWeights[1].w[4];
					E += abs(Sygma);
				}
			}
			cout << "Y=(";
			for (int k = 0; k <= 10; k++)
			{
				cout << Y[k] << ",";
			}
			cout << ")" << " W=(";

			for (int k = 3; k >= 0; k--)
			{
				cout << SynWeights[0].w[k] << ", ";
			}
			cout << SynWeights[0].w[4];

			cout << ")" << "  E=" << E << "  inc=" << (inc + 1) << endl;
			inc++;
		} while ((E != 0) && (inc != 100));
		//
		//Testing our neuronet
		//
		cout << "Подаем на НС для тестирования остальные вектора значений:" << endl;
		for (int k = 11; k <= 15; k++)
		{
			net = 0;
			for (int i = 0; i <= 3; i++)
			{
				net = net + SynWeights[0].w[i] * BinEnters[k].x[i];
			}
			net += SynWeights[0].w[4];
			out = F(net, FA);
			if (out >= 0.5) Y[k] = 1; else Y[k] = 0;
		}

		cout << "Y=(";
		for (int k = 11; k <= 15; k++)
		{
			if (Y[k] != OriginalBoolFuncValue[k]) test = 1;
			cout << Y[k] << ",";
		}
		cout << ")";
		if (test == 1) cout << endl << "НС настроена неправильно, обучающая выборка некорректна." << endl;
		else cout << endl << "НС настроена правильно." << endl;
	}
	return 0;
}