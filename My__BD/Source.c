#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define DontDeleted 0
#define TempDeleted 1
#define TotalDeleted 2

#define LEN 40
#define MAXSlavesINorg 5

#define Master_data_file_name "Master_data.txt" //master
#define Master_shift_file_name "Master_shift.txt" //master shift
#define Slave_data_file_name "Slave_data.txt" //slave

//slave
typedef struct Slave_struct {
	int id;
	char firstname[LEN];
	char lastname[LEN];
	char nickname[LEN];
	short deleted;
	int MasterID;
}Slave;

//master
struct My_date {
	int day;
	int month;
	int year;
};
typedef struct Master_struct {
	int key;
	char name[LEN];
	char country[LEN];
	struct My_date date;
	short deleted;
	int Slaves[MAXSlavesINorg];
}Master;
typedef struct Master_shift_struct {
	int key;
	int address;
	short deleted;
}Master_shift;

//slave
void print_S(Slave* print) {
	if (print->deleted == DontDeleted)
		printf("\n\nid: %d, %s :%s (%s)\nMaster Key: %d\n", print->id, print->firstname, print->lastname, print->nickname , print->MasterID);
	else if (print->deleted == TempDeleted)
		printf("\n\nid: %d, %s :%s (%s)\n", print->id, print->firstname, print->lastname, print->nickname);
	else
		printf("\n\Slave not found!\n");
	return;
}
void print_S_All()
{
	FILE* f = fopen(Slave_data_file_name, "rb");
	Slave print;
	int Lastid = -1;
	while (!(feof(f)))
	{
		fread(&print, sizeof(Slave), 1, f);
		if (Lastid != print.id)
		{
			print_S(&print);
		}
		Lastid = print.id;
	}
	fclose(f);
	return;
}
void Insert_S() {
	printf("Creating Slave;\nEnter id, firstname, lastname, nickname:\n");
	FILE* f = fopen(Slave_data_file_name, "rb+");
	Slave temp;
	scanf("%d%s%s%s", &(temp.id), &(temp.firstname), &(temp.lastname), &(temp.nickname));
	temp.deleted = TempDeleted; // no active
	fseek(f, 0, SEEK_END);
	fwrite(&temp, sizeof(Slave), 1, f);
	printf("\nSuccess Insert!\n\n");
	fclose(f);
	return;
}

//master
void shift_update(Master* temp)
{
	FILE* f_s = fopen(Master_shift_file_name, "rb+");
	Master_shift arr[100];
	fseek(f_s, 0, SEEK_SET);
	int i;
	short shifts = 0;
	for (i = 1; !feof(f_s); i++)
	{
		fread(&arr[i], sizeof(Master_shift), 1, f_s);
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
	FILE* fw = fopen(Master_shift_file_name, "wb");
	fseek(fw, 0, SEEK_SET);
	for (int j = 1; arr[j].key != -1; j++)
	{
		if (arr[j].deleted == TotalDeleted)
			continue;
		fwrite(&arr[j], sizeof(Master_shift), 1, fw);
	}

	fclose(fw);
}
void Delete_M_Perm()
{
	printf("\nDeleting Master;\nEnter Master key:");
	int key_;
	scanf("%d", &key_);
	FILE* f = fopen(Master_data_file_name, "rb+");
	Master arr[100];
	fseek(f, 0, SEEK_SET);
	int i;
	double founded = false;
	for (i = 1; !feof(f); i++)
	{
		fread(&arr[i], sizeof(Master), 1, f);
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
	FILE* fw = fopen(Master_data_file_name, "wb");
	fseek(fw, 0, SEEK_SET);
	for (int j = 1; arr[j].key != -1; j++)
	{
		if (arr[j].deleted == TotalDeleted)
		{
			shift_update(&arr[j]);
			continue;
		}
		fwrite(&arr[j], sizeof(Master), 1, fw);
	}
	fclose(fw);
	return;
}

void Delete_M_temp()
{
	printf("\nTemp deleting Master;\nEnter Master key:");
	int key_;
	scanf("%d", &key_);
	FILE* f = fopen(Master_data_file_name, "rb+");
	Master arr[100];
	fseek(f, 0, SEEK_SET);
	int i;
	bool founded = false;
	for (i = 1; !feof(f); i++)
	{
		fread(&arr[i], sizeof(Master), 1, f);
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
	FILE* fw = fopen(Master_data_file_name, "wb");
	fseek(fw, 0, SEEK_SET);
	for (int j = 1; arr[j].key != -1; j++)
	{
		fwrite(&arr[j], sizeof(Master), 1, fw);
	}
	fclose(fw);
	return;
}
void Recovery_M() 
{
	printf("\nRecover Master;\nEnter Master key:");
	int key_;
	scanf("%d", &key_);
	FILE* f = fopen(Master_data_file_name, "rb+");
	Master arr[100];
	fseek(f, 0, SEEK_SET);
	int i;
	bool founded = false;
	for (i = 1; !feof(f); i++)
	{
		fread(&arr[i], sizeof(Master), 1, f);
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
	FILE* fw = fopen(Master_data_file_name, "wb");
	fseek(fw, 0, SEEK_SET);
	for (int j = 1; arr[j].key != -1; j++)
	{
		fwrite(&arr[j], sizeof(Master), 1, fw);
	}
	fclose(fw);
	return;
}

void print_M(Master* print)
{
	if (print->deleted == DontDeleted)
		printf("\n\nKey:%d, Name:%s, Country:%s, Foundation date: %d.%d.%d\nStatus: Active!\n", print->key, print->name, print->country, print->date.day, print->date.month, print->date.year);
	else if (print->deleted == TempDeleted)
		printf("\nKey:%d, Name:%s\nStatus: temporary Deleted!\n", print->key, print->name);
	else
		printf("\n\nMaster not found!\n");
	return;
}
void print_M_Sift(Master_shift* print) 
{
	if (print->deleted == DontDeleted)
		printf("\n\nKey:%d, Adress:%d\nStatus: Active!\n", print->key, print->address);
	else if (print->deleted == TempDeleted)
		printf("\n\nKey:%d, Adress:%d\nStatus: temporary Deleted!\n", print->key, print->address);
	else
		printf("\n\nMaster_shift not found!\n");
	return;
}
void print_M_All()
{
	FILE* f = fopen(Master_data_file_name, "rb");
	Master print;
	int LastKey = -1;
	while (!(feof(f)))
	{
		fread(&print, sizeof(Master), 1, f);
		if (LastKey != print.key)
		{
			print_M(&print);
		}
		LastKey = print.key;
	}
	fclose(f);
	return;
}
void print_M_AllShift()
{
	FILE* f = fopen(Master_shift_file_name, "rb");
	Master_shift print;
	int LastKey = -1;
	while (!(feof(f)))
	{
		fread(&print, sizeof(Master_shift), 1, f);
		if (LastKey != print.key)
		{
			print_M_Sift(&print);
		}
		LastKey = print.key;
	}
	fclose(f);
	return;
}

void Insert_M()
{
	printf("\nCreating Master;\nEnter key, name, country, fundation data(dd mm yyyy):\n");
	FILE* f = fopen(Master_data_file_name, "rb+");
	Master temp;
	scanf("%d%s%s%d%d%d", &(temp.key), &(temp.name), &(temp.country), &(temp.date.day), &(temp.date.month), &(temp.date.year));
	temp.deleted = DontDeleted;
	for (int q = 0; q < MAXSlavesINorg; q++)
	{
		temp.Slaves[q] = 0;
	}
	fseek(f, 0, SEEK_END);
	fwrite(&temp, sizeof(Master), 1, f);

	FILE* f_s = fopen(Master_shift_file_name, "rb+");
	Master_shift temp_s;
	temp_s.key = temp.key;
	temp_s.address = ftell(f) / sizeof(Master);
	temp_s.deleted = DontDeleted;
	fseek(f_s, 0, SEEK_END);
	fwrite(&temp_s, sizeof(Master_shift), 1, f_s);
	printf("\n");
	print_M(&temp);
	printf("\nSuccess Insert!\n\n");
	fclose(f);
	fclose(f_s);
	return;
}
void Update_M()
{
	printf("\nUpdating Master;\nEnter Master key:\n");
	FILE* f_s = fopen(Master_shift_file_name, "rb");
	int key_;
	scanf("%d", &key_);
	Master_shift temp_s;
	bool flag = 0;
	while (!feof(f_s))
	{
		fread(&temp_s, sizeof(Master_shift), 1, f_s);
		if (temp_s.key == key_)
		{
			flag = 1;
			break;
		}
	}
	if (!flag)
	{
		printf("Master don't found!\n");
		return;
	}
	FILE* f = fopen(Master_data_file_name, "rb");
	Master temp;
	Master scan;
	fseek(f, sizeof(Master) * (temp_s.address - 1), 1, SEEK_SET);
	fread(&temp, sizeof(Master), 1, f);
	if (temp.deleted == TempDeleted)
	{
		print_M(&temp);
		return;
	}
	printf("\nEnter name(0 if don't change):");
	scanf("%s", &(scan.name));
	printf("\nEnter country(0 if don't change):");
	scanf("%s", &(scan.country));
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
	FILE* fw = fopen(Master_data_file_name, "rb+");
	fseek(fw, sizeof(Master) * (temp_s.address - 1), 1, SEEK_SET);
	fwrite(&temp, sizeof(Master), 1, fw);
	print_M(&temp);
	printf("Success Update!\n");
	fclose(fw);
	return;
}

Master Get_M(int key_) {
	FILE* f_s = fopen(Master_shift_file_name, "rb");
	Master_shift temp_s;
	bool flag = 0;
	while (!feof(f_s)) {
		fread(&temp_s, sizeof(Master_shift), 1, f_s);
		if (temp_s.key == key_) {
			flag = 1;
			break;
		}
	}
	fclose(f_s);
	Master temp_d;
	temp_d.name[0] = '-1';
	temp_d.country[0] = '-1';
	temp_d.date.day = -1;
	temp_d.date.month = -1;
	temp_d.date.year = -1;
	temp_d.deleted = TotalDeleted;
	if (!flag) {
		return temp_d;
	}
	FILE* f = fopen(Master_data_file_name, "rb");
	Master temp;
	fseek(f, sizeof(Master) * (temp_s.address - 1), 1, SEEK_SET);
	fread(&temp, sizeof(Master), 1, f);
	fclose(f);
	return temp;
}
void Print_M() {
	printf("\nGet(print) Master;\nEnter Master key:");
	FILE* f_s = fopen(Master_shift_file_name, "rb");
	int key_;
	scanf("%d", &key_);
	Master_shift temp_s;
	bool flag = 0;
	//int ch = getc(f_s);
	// (ch != EOF)
	while (!feof(f_s)) {
		fread(&temp_s, sizeof(Master_shift), 1, f_s);
		if (temp_s.key == key_) {
			flag = 1;
			break;
		}
	}
	fclose(f_s);
	Master temp_d;
	temp_d.name[0] = '-1';
	temp_d.country[0] = '-1';
	temp_d.date.day = -1;
	temp_d.date.month = -1;
	temp_d.date.year = -1;
	temp_d.deleted = TotalDeleted;
	if (!flag) {
		print_M(&temp_d);
		return;
	}
	FILE* f = fopen(Master_data_file_name, "rb");
	Master temp;
	fseek(f, sizeof(Master) * (temp_s.address - 1), 1, SEEK_SET);
	fread(&temp, sizeof(Master), 1, f);
	fclose(f);
	print_M(&temp);
	return;
}

int main() {
	while (1) {
		Insert_S();
		Insert_S();
		Insert_S();
		print_S_All();
		system("pause");
	}
	return 0;
}
//---------------------------------------------Functional

	//print_M_All(); //work +
	//print_M_AllShift(); //work +

	//Insert_M();  //work + 
	//Update_M(); //work +

	//Print_M(); //work +
	//Master My = Get_M();	//work +
	//print_M(&My);  //work +

	//Delete_M_temp(); //work +
	//Recovery_M(); //work +

	//Delete_M_Perm(); //work +
/*
organisation + client.
Master(organisation file){update insert get} and Slave (Slave file){update insert get}

Slave have unique key and i can write all Slave key in Organisation array!!!
*/