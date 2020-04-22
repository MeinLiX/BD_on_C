#define _CRT_SECURE_NO_WARNINGS

#ifndef SOURCE
#define SOURCE
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define SPACE printf("\nNEXT COMMAND\n")

#define DontDeleted 0
#define TempDeleted 1
#define TotalDeleted 2

#define LEN 40
#define MAXSlavesINorg 5

#define Master_data_file_name "Master_data.txt" //master
#define Master_shift_file_name "Master_shift.txt" //master shift
#define Slave_data_file_name "Slave_data.txt" //slave

struct My_date {
	int day;
	int month;
	int year;
};

//slave STR
typedef struct Slave_struct {
	int id;
	char firstname[LEN];
	char lastname[LEN];
	char nickname[LEN];
	short deleted;
	int MasterKey;
}Slave;

//master STR

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


//slave FN
void print_S(Slave* print) {
	if (print->deleted == DontDeleted) //use Master
		printf("\n\nid: %d, %s %s (%s)\nMaster Key: %d\n", print->id, print->firstname, print->lastname, print->nickname, print->MasterKey);
	else if (print->deleted == TempDeleted) //unuse Master
		printf("\n\nid: %d, %s %s (%s)\n", print->id, print->firstname, print->lastname, print->nickname);
	else
		printf("\nSlave not found!\n");
	printf("\n\t***\t***\t***\n");
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
	temp.MasterKey = 0;
	fseek(f, 0, SEEK_END);
	fwrite(&temp, sizeof(Slave), 1, f);
	printf("\nSuccess Insert!\n\n");
	fclose(f);
	return;
}
void Update_S()
{
	printf("\nUpdating Slave;\nEnter Slave ID:\n");
	FILE* f_s = fopen(Slave_data_file_name, "rb");
	int id;
	scanf("%d", &id);
	Slave temp_s;
	int Slave_Shift;
	bool flag = 0;
	while (!feof(f_s))
	{
		fread(&temp_s, sizeof(Slave), 1, f_s);
		if (temp_s.id == id)
		{
			flag = 1;
			Slave_Shift = ftell(f_s) / sizeof(Slave);
			break;
		}
	}
	fclose(f_s);
	if (!flag)
	{
		printf("Slave don't found!\n");
		return;
	}
	Slave scan;
	printf("\nEnter First Name (0 if don't change):");
	scanf("%s", &(scan.firstname));
	printf("\nEnter Last Name (0 if don't change):");
	scanf("%s", &(scan.lastname));
	printf("\nEnter Nickname (0 if don't change):");
	scanf("%s", &(scan.nickname));
	if (strcmp(scan.firstname, "0"))
	{
		strcpy(temp_s.firstname, scan.firstname);
	}
	if (strcmp(scan.lastname, "0"))
	{
		strcpy(temp_s.lastname, scan.lastname);
	}
	if (strcmp(scan.nickname, "0"))
	{
		strcpy(temp_s.nickname, scan.nickname);
	}
	FILE* fw = fopen(Slave_data_file_name, "rb+");
	fseek(fw, sizeof(Slave) * (Slave_Shift - 1), 1, SEEK_SET);
	fwrite(&temp_s, sizeof(Slave), 1, fw);
	print_S(&temp_s);
	printf("Success Update!\n");
	fclose(fw);
	return;
}
void False_S(int id_) {
	FILE* f_s = fopen(Slave_data_file_name, "rb");
	Slave temp_s;
	int Slave_Shift;
	bool flag = 0;
	while (!feof(f_s))
	{
		fread(&temp_s, sizeof(Slave), 1, f_s);
		if (temp_s.id == id_)
		{
			flag = 1;
			Slave_Shift = ftell(f_s) / sizeof(Slave);
			break;
		}
	}
	fclose(f_s);
	if (!flag)
	{
		printf("Slave don't found!\n");
		return;
	}
	temp_s.deleted = TempDeleted;

	FILE* fw = fopen(Slave_data_file_name, "rb+");
	fseek(fw, sizeof(Slave) * (Slave_Shift - 1), 1, SEEK_SET);
	fwrite(&temp_s, sizeof(Slave), 1, fw);
	print_S(&temp_s);
	printf("Success Update!\n");
	fclose(fw);
	return;
}


Slave Get_S(int id_) {
	FILE* f = fopen(Slave_data_file_name, "rb");
	Slave res;
	bool flag = 0;
	while (!feof(f))
	{
		fread(&res, sizeof(Slave), 1, f);
		if (res.id == id_)
		{
			flag = true;
			break;
		}
	}
	if (!flag)
	{
		res.id = -1;
		res.deleted = TotalDeleted;
		return res;
	}
	return res;
}
void Get_Print_S() {
	printf("\nGet(print) Master;\nEnter Master key:");
	int id_;
	scanf("%d", &id_);
	FILE* f = fopen(Slave_data_file_name, "rb");
	Slave res;
	bool flag = 0;
	while (!feof(f))
	{
		fread(&res, sizeof(Slave), 1, f);
		if (res.id == id_)
		{
			flag = true;
			break;
		}
	}
	if (!flag)
	{
		res.id = -1;
		res.deleted = TotalDeleted;
		print_S(&res);

	}
	print_S(&res);
	return;
}

//master FN
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
			for (int q = 0; q < 5; q++) 
				False_S(arr[j].Slaves[q]);
			
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
	if (print->deleted == DontDeleted) {
		printf("\n\nKey:%d, Name:%s, Country:%s, Foundation date: %d.%d.%d\n", print->key, print->name, print->country, print->date.day, print->date.month, print->date.year);
		for (int q = 0; q < MAXSlavesINorg; q++) {
			if (print->Slaves[q]) {
				Slave temp_slave = Get_S(print->Slaves[q]);
					printf("Slave \"%s\" {%d}\n", temp_slave.nickname, temp_slave.id);
			}
		}
		printf("Status: Active!\n");
	}
	else if (print->deleted == TempDeleted)
		printf("\nKey:%d, Name:%s\nStatus: temporary Deleted!\n", print->key, print->name);
	else
		printf("\n\nMaster not found!\n");
	printf("\n\t***\t***\t***\n");
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
	printf("\n\t***\t***\t***\n");
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
void Get_Print_M() {
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

//master and slave FN
void Add_S_to_M(int key_M, int id_S) {
	FILE* f_shift = fopen(Master_shift_file_name, "rb"); //master_shift search by key
	Master_shift temp_sift;
	bool flag = 0;
	while (!feof(f_shift))
	{
		fread(&temp_sift, sizeof(Master_shift), 1, f_shift);
		if (temp_sift.key == key_M)
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
	fclose(f_shift);

	FILE* f_slave = fopen(Slave_data_file_name, "rb"); //slave search by key
	Slave temp_slave;
	int Slave_Shift;
	flag = 0;
	while (!feof(f_slave))
	{
		fread(&temp_slave, sizeof(Slave), 1, f_slave);
		if (temp_slave.id == id_S)
		{
			flag = 1;
			Slave_Shift = ftell(f_slave) / sizeof(Slave);
			break;
		}
	}
	fclose(f_slave);
	if (!flag)
	{
		printf("Slave don't found!\n");
		return;
	}
	if (temp_slave.deleted == DontDeleted) {
		printf("Slave already in Master!\n");
		return;
	}


	FILE* f = fopen(Master_data_file_name, "rb");
	Master temp_M;
	fseek(f, sizeof(Master) * (temp_sift.address - 1), 1, SEEK_SET);
	fread(&temp_M, sizeof(Master), 1, f);
	fclose(f);

	flag = 0;
	for (int q = 0; q < MAXSlavesINorg; q++) {
		if (temp_M.Slaves[q] == 0) {
			temp_M.Slaves[q] = temp_slave.id;
			flag = true;
			break;
		}
	}
	if (!flag) {
		printf("Master have max Slave's!\n");
		return;
	}
	temp_slave.deleted = DontDeleted;
	temp_slave.MasterKey = temp_M.key;

	FILE* f_slave_w = fopen(Slave_data_file_name, "rb+");
	fseek(f_slave_w, sizeof(Slave) * (Slave_Shift - 1), 1, SEEK_SET);
	fwrite(&temp_slave, sizeof(Slave), 1, f_slave_w);
	fclose(f_slave_w);


	//here push new temp
	FILE* f_master_w = fopen(Master_data_file_name, "rb+");
	fseek(f_master_w, sizeof(Master) * (temp_sift.address - 1), 1, SEEK_SET);
	fwrite(&temp_M, sizeof(Master), 1, f_master_w);
	fclose(f_master_w);

	printf("\nSlave %s (id: %d) added to Master %s (key: %d)!\n", temp_slave.nickname, temp_slave.id, temp_M.name, temp_M.key);
	return;
}
void Remove_S_from_M(int key_M, int id_S) {
	FILE* f_shift = fopen(Master_shift_file_name, "rb"); //master_shift search by key
	Master_shift temp_sift;
	bool flag = 0;
	while (!feof(f_shift))
	{
		fread(&temp_sift, sizeof(Master_shift), 1, f_shift);
		if (temp_sift.key == key_M)
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
	fclose(f_shift);

	FILE* f_slave = fopen(Slave_data_file_name, "rb"); //slave search by key
	Slave temp_slave;
	int Slave_Shift;
	flag = 0;
	while (!feof(f_slave))
	{
		fread(&temp_slave, sizeof(Slave), 1, f_slave);
		if (temp_slave.id == id_S)
		{
			flag = 1;
			Slave_Shift = ftell(f_slave) / sizeof(Slave);
			break;
		}
	}
	fclose(f_slave);
	if (!flag)
	{
		printf("Slave don't found!\n");
		return;
	}
	if (temp_slave.deleted == TempDeleted) {
		printf("Slave don't used Master!\n");
		return;
	}


	FILE* f = fopen(Master_data_file_name, "rb");
	Master temp_M;
	fseek(f, sizeof(Master) * (temp_sift.address - 1), 1, SEEK_SET);
	fread(&temp_M, sizeof(Master), 1, f);
	fclose(f);
	//here changes temp
	flag = 0;
	for (int q = 0; q < MAXSlavesINorg; q++) {
		if (temp_M.Slaves[q] == temp_slave.id) {
			temp_M.Slaves[q] = 0;
			flag = true;
			break;
		}
	}
	if (!flag) {
		printf("No Slave was found in Master by this ID!\n");
		return;
	}
	temp_slave.deleted = TempDeleted;

	FILE* f_slave_w = fopen(Slave_data_file_name, "rb+");
	fseek(f_slave_w, sizeof(Slave) * (Slave_Shift - 1), 1, SEEK_SET);
	fwrite(&temp_slave, sizeof(Slave), 1, f_slave_w);
	fclose(f_slave_w);


	//here push new temp
	FILE* fw = fopen(Master_data_file_name, "rb+");
	fseek(fw, sizeof(Master) * (temp_sift.address - 1), 1, SEEK_SET);
	fwrite(&temp_M, sizeof(Master), 1, fw);
	printf("\nSlave %s (id: %d) removed from Master %s (key: %d)!\n", temp_slave.nickname, temp_slave.id, temp_M.name, temp_M.key);
	fclose(fw);
	return;
}

int main() {
	while (1) {
		system("pause");
	}
	return 0;
}

/*
1) Заздалегідь введіть 2 чи 3 master records з slave records у деяких  master records;
Add_S_to_M(1, 1);
		Add_S_to_M(1, 2);
		Add_S_to_M(2, 3);
		Add_S_to_M(3, 4);
		Add_S_to_M(3, 5);
2) показати списки обох типів записів;
3) додати 1 master record і 2 slave records;
4) вилучити 1 master record з тих, що були раніше;
5) показати списки обох типів записів;
6) оновити значення неключового поля в master record, яка була введена на 3)кроці;
7) показати результат оновлення;
8) вилучити 1 slave record(будь-яку);
9) показати списки обох типів записів.
*/


//---------------------------------------------Functional

//master and slave FN

//Add_S_to_M(int key_M, int id_S) //work +
//Remove_S_from_M(int key_M, int id_S) //work +


//slave FN

//print_S(Slave* print) //work +
//print_S_All() //work +

//Insert_S() //work +
//Update_S() //work +

//Slave Get_S(int id_) //work +
//Get_Print_S() //work +


////master FN

//shift_update(Master* temp) //work +
//Delete_M_Perm() //work +

//Delete_M_temp() //work +
//Recovery_M() //work +

//print_M(Master* print) //work +
//print_M_Sift(Master_shift* print) //work +
//print_M_All() //work +
//print_M_AllShift() //work +

//Insert_M() //work +
//Update_M() //work +

//Master Get_M(int key_) //work +
//Get_Print_M() //work +



	
/*
organisation + client.
Master(organisation file){update insert get} and Slave (Slave file){update insert get}

Slave have unique key and i can write all Slave key in Organisation array!!!
*/
#endif