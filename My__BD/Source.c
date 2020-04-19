#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TempDeleted 1
#define TotalDeleted 2
#define DontDeleted 0
#define LEN 40
#define MAXplayersINorg 5
#define Organisation_data_file_name "Organisation_data.txt" //master
#define Organisation_shift_file_name "Organisation_shift.txt" //master
#define Player_data_file_name "Player_data.txt" //slave

//slave
typedef struct Player {
	int id;
	char firstname[LEN];
	char lastname[LEN];
	char nickname[LEN];
	short deleted;
	int compID;
}player;

//master
struct My_date {
	int day;
	int month;
	int year;
};
typedef struct Ñompany {
	int key;
	char name[LEN];
	char country[LEN];
	struct My_date date;
	short deleted;
	int players[MAXplayersINorg];
}comp;
typedef struct Ñompany_shift {
	int key;
	int address;
	short deleted;
}comp_shift;

//slave
/*void Insert_S() {//Organisation
	printf("Creating Player;\nEnter key, firstname, lastname, nickname:\n");
	FILE* f = fopen(Player_data_file_name, "rb+");
	player temp;
	scanf("%d%40s%40s%40s", &(temp.id), &(temp.firstname), &(temp.lastname), &(temp.nickname));
	temp.deleted = DontDeleted;
	fseek(f, 0, SEEK_END);
	fwrite(&temp, sizeof(player), 1, f);

	printf("\n");
	printf("Success Insert!\n\n");
	fclose(f);
	return;
}
*/


//master
void shift_update(comp* temp)
{
	FILE* f_s = fopen(Organisation_shift_file_name, "rb+");
	comp_shift arr[100];
	fseek(f_s, 0, SEEK_SET);
	int i;
	short shifts = 0;
	for (i = 1; !feof(f_s); i++)
	{
		fread(&arr[i], sizeof(comp_shift), 1, f_s);
		if (arr[i].key == temp->key)
		{
			arr[i].deleted = TotalDeleted;
			shifts = 1;
		}
		else if
			(shifts)arr[i].address -= shifts;
	}
	arr[i - 1].key = -1;
	fclose(f_s);
	if (!shifts)
		return;
	FILE* fw = fopen(Organisation_shift_file_name, "wb");
	fseek(fw, 0, SEEK_SET);
	for (int j = 1; arr[j].key != -1; j++)
	{
		if (arr[j].deleted == TotalDeleted)
			continue;
		fwrite(&arr[j], sizeof(comp_shift), 1, fw);
	}

	fclose(fw);
}
void Delete_M_Merm()
{
	printf("Deleting Organisation;\nEnter organisation key:");
	int key_;
	scanf("%d", &key_);
	FILE* f = fopen(Organisation_data_file_name, "rb+");
	comp arr[100];
	fseek(f, 0, SEEK_SET);
	int i;
	double founded = false;
	for (i = 1; !feof(f); i++)
	{
		fread(&arr[i], sizeof(comp), 1, f);
		if (arr[i].key == key_)
		{
			arr[i].deleted = TotalDeleted;
			founded = true;
		}
	}
	arr[i - 1].key = -1;
	fclose(f);
	if (!founded)
	{
		printf("\nThe key was not found!\n");
		return;
	}
	FILE* fw = fopen(Organisation_data_file_name, "wb");
	fseek(fw, 0, SEEK_SET);
	for (int j = 1; arr[j].key != -1; j++)
	{
		if (arr[j].deleted == TotalDeleted)
		{
			shift_update(&arr[j]);
			continue;
		}
		fwrite(&arr[j], sizeof(comp), 1, fw);
	}
	fclose(fw);
	return;
}

void Delete_M_temp()
{
	printf("Temp deleting Organisation;\nEnter organisation key:");
	int key_;
	scanf("%d", &key_);
	FILE* f = fopen(Organisation_data_file_name, "rb+");
	comp arr[100];
	fseek(f, 0, SEEK_SET);
	int i;
	bool founded = false;
	for (i = 1; !feof(f); i++)
	{
		fread(&arr[i], sizeof(comp), 1, f);
		if (arr[i].key == key_)
		{
			founded = true;
			arr[i].deleted = TempDeleted;
		}
	}fclose(f);
	if (!founded)
	{
		printf("\nThe key was not found!\n");
		return;
	}
	arr[i - 1].key = -1;
	FILE* fw = fopen(Organisation_data_file_name, "wb");
	fseek(fw, 0, SEEK_SET);
	for (int j = 1; arr[j].key != -1; j++)
	{
		fwrite(&arr[j], sizeof(comp), 1, fw);
	}
	fclose(fw);
	return;
}
void Remove_Delete_M_temp() 
{
	printf("Recover Organisation;\nEnter organisation key:");
	int key_;
	scanf("%d", &key_);
	FILE* f = fopen(Organisation_data_file_name, "rb+");
	comp arr[100];
	fseek(f, 0, SEEK_SET);
	int i;
	bool founded = false;
	for (i = 1; !feof(f); i++)
	{
		fread(&arr[i], sizeof(comp), 1, f);
		if (arr[i].key == key_)
		{
			founded = true;
			arr[i].deleted = DontDeleted;
		}
	}
	fclose(f);
	if (!founded)
	{
		printf("\nThe key was not found!\n");
		return;
	}
	arr[i - 1].key = -1;
	FILE* fw = fopen(Organisation_data_file_name, "wb");
	fseek(fw, 0, SEEK_SET);
	for (int j = 1; arr[j].key != -1; j++)
	{
		fwrite(&arr[j], sizeof(comp), 1, fw);
	}
	fclose(fw);
	return;
}

void printComp(comp* print)
{
	if (print->deleted == DontDeleted)
		printf("\n\nKey:%d, Name:%s, Country:%s, Foundation date: %d.%d.%d\nStatus: Active!\n", print->key, print->name, print->country, print->date.day, print->date.month, print->date.year);
	else if (print->deleted == TempDeleted)
		printf("\nKey:%d, Name:%s\nStatus: temporary Deleted!\n", print->key, print->name);
	else
		printf("\n\nStruct not found!\n");
	return;
}
void printCompS(comp_shift* print) 
{
	if (print->deleted == DontDeleted)
		printf("\n\nKey:%d, Adress:%d\nStatus: Active!\n", print->key, print->address);
	else if (print->deleted == TempDeleted)
		printf("\n\nKey:%d, Adress:%d\nStatus: temporary Deleted!\n", print->key, print->address);
	else
		printf("\n\nStruct not found!\n");
	return;
}
void printCompAll()
{
	FILE* f = fopen(Organisation_data_file_name, "rb");
	comp print;
	int LastKey = -1;
	while (!(feof(f)))
	{
		fread(&print, sizeof(comp), 1, f);
		if (LastKey != print.key)
		{
			printComp(&print);
		}
		LastKey = print.key;
	}
	fclose(f);
	return;
}
void printCompAllS()
{
	FILE* f = fopen(Organisation_shift_file_name, "rb");
	comp_shift print;
	int LastKey = -1;
	while (!(feof(f)))
	{
		fread(&print, sizeof(comp_shift), 1, f);
		if (LastKey != print.key)
		{
			printCompS(&print);
		}
		LastKey = print.key;
	}
	fclose(f);
	return;
}

void Insert_M()
{
	printf("Creating Organisation;\nEnter key, name, country, fundation data(dd mm yyyy):\n");
	FILE* f = fopen(Organisation_data_file_name, "rb+");
	comp temp;
	scanf("%d%40s%40s%d%d%d", &(temp.key), &(temp.name), &(temp.country), &(temp.date.day), &(temp.date.month), &(temp.date.year));
	temp.deleted = DontDeleted;
	for (int q = 0; q < MAXplayersINorg; q++)
	{
		temp.players[q] = 0;
	}
	fseek(f, 0, SEEK_END);
	fwrite(&temp, sizeof(comp), 1, f);

	FILE* f_s = fopen(Organisation_shift_file_name, "rb+");
	comp_shift temp_s;
	temp_s.key = temp.key;
	temp_s.address = ftell(f) / sizeof(comp);
	temp_s.deleted = DontDeleted;
	fseek(f_s, 0, SEEK_END);
	fwrite(&temp_s, sizeof(comp_shift), 1, f_s);
	printf("\n");
	printComp(&temp);
	printf("Success Insert!\n\n");
	fclose(f);
	fclose(f_s);
	return;
}
void Update_M()
{
	printf("Updating Organisation;\nEnter organisation key(for update data):\n");
	FILE* f_s = fopen(Organisation_shift_file_name, "rb");
	int key_;
	scanf("%d", &key_);
	comp_shift temp_s;
	bool flag = 0;
	while (!feof(f_s))
	{
		fread(&temp_s, sizeof(comp_shift), 1, f_s);
		if (temp_s.key == key_)
		{
			flag = 1;
			break;
		}
	}
	if (!flag)
	{
		printf("Struct don't founded!\n");
		return;
	}
	FILE* f = fopen(Organisation_data_file_name, "rb");
	comp temp;
	comp scan;
	fseek(f, sizeof(comp) * (temp_s.address - 1), 1, SEEK_SET);
	fread(&temp, sizeof(comp), 1, f);
	if (temp.deleted == TempDeleted)
	{
		printComp(&temp);
		return;
	}
	printf("\nEnter name(0 if don't change):");
	scanf("%40s", &(scan.name));
	printf("\nEnter country(0 if don't change):");
	scanf("%40s", &(scan.country));
	printf("\nEnter date(dd mm yyyy)(0 0 0 if don't change):");
	scanf("%d%d%d", &(scan.date.day), &(scan.date.month), &(scan.date.year));
	if (strcmp(scan.name, "0"))
	{
		strcpy(temp.name, scan.name);
	}
	if (strcmp(scan.country, "0"))
	{
		strcpy(temp.country, scan.country);
	}
	if (scan.date.day && scan.date.month && scan.date.month)
	{
		temp.date.day = scan.date.day;
		temp.date.month = scan.date.month;
		temp.date.year = scan.date.year;
	}
	fclose(f);
	//Delete_T_Mointer_f(temp_s.address); //don't need
	FILE* fw = fopen(Organisation_data_file_name, "rb+");
	fseek(fw, sizeof(comp) * (temp_s.address - 1), 1, SEEK_SET);
	fwrite(&temp, sizeof(comp), 1, fw);
	printComp(&temp);
	printf("Success Update!\n");
	fclose(fw);
	return;
}

comp Get_M_r(int key_) {
	FILE* f_s = fopen(Organisation_shift_file_name, "rb");
	comp_shift temp_s;
	bool flag = 0;
	while (!feof(f_s)) {
		fread(&temp_s, sizeof(comp_shift), 1, f_s);
		if (temp_s.key == key_) {
			flag = 1;
			break;
		}
	}
	fclose(f_s);
	comp temp_d;
	temp_d.name[0] = '-1';
	temp_d.country[0] = '-1';
	temp_d.date.day = -1;
	temp_d.date.month = -1;
	temp_d.date.year = -1;
	temp_d.deleted = TotalDeleted;
	if (!flag) {
		return temp_d;
	}
	FILE* f = fopen(Organisation_data_file_name, "rb");
	comp temp;
	fseek(f, sizeof(comp) * (temp_s.address - 1), 1, SEEK_SET);
	fread(&temp, sizeof(comp), 1, f);
	fclose(f);
	return temp;
}
void Get_M() {
	printf("Enter organisation key(for get data):\n");
	FILE* f_s = fopen(Organisation_shift_file_name, "rb");
	int key_;
	scanf("%d", &key_);
	comp_shift temp_s;
	bool flag = 0;
	//int ch = getc(f_s);
	// (ch != EOF)
	while (!feof(f_s)) {
		fread(&temp_s, sizeof(comp_shift), 1, f_s);
		if (temp_s.key == key_) {
			flag = 1;
			break;
		}
	}
	fclose(f_s);
	comp temp_d;
	temp_d.name[0] = '-1';
	temp_d.country[0] = '-1';
	temp_d.date.day = -1;
	temp_d.date.month = -1;
	temp_d.date.year = -1;
	temp_d.deleted = TotalDeleted;
	if (!flag) {
		printComp(&temp_d);
		return;
	}
	FILE* f = fopen(Organisation_data_file_name, "rb");
	comp temp;
	fseek(f, sizeof(comp) * (temp_s.address - 1), 1, SEEK_SET);
	fread(&temp, sizeof(comp), 1, f);
	fclose(f);
	printComp(&temp);
	return;
}

int main() {
	while (1) {
		printCompAll();
		//printCompAllS();
		system("pause");
	}
	return 0;
}
//---------------------------------------------Functional

	//printCompAll(); //work +
	//printCompAllS(); //work +
	//Insert_M();  //work + 
	//Update_M(); //work +
	//Get_M(); //work +
	//Get_M_r(); //work +

	//comp My = Get_M();	//work +
	//printComp(&My);  //work +

	//Delete_M_temp(); //work +
	//Remove_Delete_M_temp(); //work +

	//Delete_M_Merm(); //work +

	//temp_total_delete(); //cleaner need realise

/*
organisation + client.
Master(organisation file){update insert get} and Slave (player file){update insert get}

Slave have unique key and i can write all player key in Organisation array!!!
*/