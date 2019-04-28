#include <fstream>
#include <iostream>
#include <iomanip>
#include <cstdio>  
#include <chrono>     
#include <thread>
#include <string> 
#include <sstream>

using namespace std;

void CalcInput(double, double);
char getOperationByValue(int);
double CalculateOperation(double, double, int);

double totalCount = 0.0;
double lastValue = 0;
double lastOperationValue = 0, currentOperationValue = 0;

int main(int argc, char* argv[])
{
	string line;

	int seekCount;
	int count = 0;
	int nList = 0;
	int nIndex = 0;
	int nFiles = 0;
	char operationSymbol;

	//TaskList
	ifstream taskList;
	taskList.open("c:\\taskList", ios::in | ios::binary);

	//TaskIndex
	ifstream  taskIndex;
	taskIndex.open("c:\\taskIndex", ios::in | ios::binary);

	//TaskFile
	ifstream  taskFile;
	taskFile.open("c:\\taskFile", ios::in | ios::binary | ios::ate);

	//taskOutputFile
	ofstream ofile;
	ofile.open("c:\\taskOutput.txt", ios::app);


	while (!taskList.eof())
	{
		getline(taskList, line);

		stringstream   linestream(line);
		while (linestream >> nList) {

			//Indexte ki okunacak byte bulunuyor. ilk dört byte bizim nList deðerini döndürdüðü için son 4 byte lýk deðeri okuyoruz.
			seekCount = nList * 8 + 4;

			//TaskList'te ki sýraya göre index tablosundan ilgili sonucu alýyoruz.
			taskIndex.clear();
			taskIndex.seekg(seekCount, ios::beg);
			taskIndex.read(reinterpret_cast<char*>(&nIndex), sizeof(4));

			//Sonucun File dosyasýndaki karþýlýðýný alýyoruz.
			taskFile.clear();
			taskFile.seekg(nIndex, ios::beg);
			taskFile.read(reinterpret_cast<char*>(&nFiles), sizeof(4));

			count++;
			//Operatörler 2. okumada alýnýyor.
			if (count % 2 == 0) {
				operationSymbol = getOperationByValue(nFiles);
				ofile << operationSymbol << " ";
				//current Operation = nfiles
				currentOperationValue = nFiles;
			}
			else {
				//tekli okumalar her zaman 4 bytelýk sayýlarýmýz 
				ofile << nFiles << " ";
				//ilk okuma için önünde parametre olmadýðý için bir iþlem.
				if (totalCount == 0) {
					totalCount = nFiles;
				}
				else {
					//okunan deðeri ve iþlemi hesaplamak için ilgili fonksiyonumuzu çalýþtýrýyoruz.
					CalcInput(currentOperationValue, nFiles);
				}
			}
		}
		//cout << endl << totalCount << ' ';

		ofile << " = " << totalCount << endl;
		totalCount = 0;
		count = 0;
		lastValue = 0;
		lastOperationValue = 0;
		currentOperationValue = 0;
	}

	// açýlan dosyalar kapanýyor.
	ofile.close();
	taskList.close();
	taskIndex.close();
	taskFile.close();
	cout << endl;
	system("pause");
	return 0;
}

// Hesaplama yapýlacak operatör ve sayý gönderiliyor.
void CalcInput(double operatorCount, double value)
{
	//Eðer iþlemimiz çarpma ise matematiksel iþlemlerin önceliði için 
	if ((operatorCount == -3 || operatorCount == -4) && (lastOperationValue == -1 || lastOperationValue == -2))
	{
		totalCount = (double)CalculateOperation(totalCount, lastValue, lastOperationValue == -1 ? -2 : -1); 
		lastValue = CalculateOperation(lastValue, value, operatorCount);
		totalCount = (double)CalculateOperation(totalCount, lastValue, lastOperationValue);
	}
	else {
		totalCount = (double)CalculateOperation(totalCount, value, operatorCount);
		lastValue = value;
		lastOperationValue = operatorCount;
	}

}


//+ -1, - -2, * -3 , /-4 olacak þekilde value1 ile value2 yi iþleme sokan fonksiyon
double CalculateOperation(double value1, double value2, int operatorCount)
{
	double result = 0;
	if (operatorCount == -1) {
		result = value1 + value2;
	}
	else if (operatorCount == -2)
	{
		result = value1 - value2;
	}
	else if (operatorCount == -3)
	{
		result = value1 * value2;
	}
	else if (operatorCount == -4)
	{
		result = value1 / value2;
	}
	return result;
}

//ilgili tipe göre karakter olarak sonucu döndürüyor
char getOperationByValue(int value) {
	if (value == -1) {
		return '+';
	}
	else if (value == -2) {
		return '-';
	}
	else if (value == -3)
	{
		return '*';
	}
	else if (value == -4)
	{
		return '/';
	}
	return '*';
}