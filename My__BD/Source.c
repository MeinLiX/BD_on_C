#define _CRT_SECURE_NO_WARNINGS
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TempDeleted 1
#define TotalDeleted 2
#define DontDeleted 0
#define LEN 40
#define Organisation_data_file_name "Organisation_data.txt"
#define Organisation_shift_file_name "Organisation_shift.txt"

struct My_date {
	int day;
	int month;
	int year;
};
typedef struct company {
	int key;
	char name[LEN];
	char street[LEN];
	struct My_date date;
	short deleted;
	int priority;
}comp;
typedef struct company_shift {
	int key;
	int address;
	short deleted;
}comp_s;


void shift_update(comp* temp) {
	FILE* f_s = fopen(Organisation_shift_file_name, "rb+");
	comp_s arr[100];
	fseek(f_s, 0, SEEK_SET);
	int i;
	short shifts = 0;
	for (i = 1; !feof(f_s); i++) {
		if (i == temp->key) {
			fread(&arr[i], sizeof(comp_s), 1, f_s);
			arr[i].key = 0;
			shifts = 1;
		}
		else {
			fread(&arr[i], sizeof(comp_s), 1, f_s);
			if (shifts)arr[i].address -= sizeof(comp_s);
		}
	}
	fclose(f_s);
	FILE* fw = fopen(Organisation_shift_file_name, "wb");
	fseek(fw, 0, SEEK_SET);
	for (int j = 1; arr[j].key != -1; j++) {
		if (arr[j].key == 0) {
			continue;
		}
		fwrite(&arr[j], sizeof(comp), 1, fw);
	}

	fclose(fw);
}
void Delete_O_perm_key(int key_) {
	FILE* f = fopen(Organisation_data_file_name, "rb+");
	comp arr[100];
	fseek(f, 0, SEEK_SET);
	int i;
	for (i = 1; !feof(f); i++) {
		fread(&arr[i], sizeof(comp), 1, f);
		if (arr[i].key == key_) {
			//fread(&arr[i], sizeof(comp), 1, f);
			arr[i].key = 0;
		}
	}
	arr[i - 1].key = -1;
	fclose(f);
	FILE* fw = fopen(Organisation_data_file_name, "wb");
	fseek(fw, 0, SEEK_SET);
	short shiftbool = 0;
	for (int j = 1; arr[j].key != -1; j++) {
		if (arr[j].key == 0) {
			continue;
			shiftbool = 1;
		} if (shiftbool)shift(&arr[j]);
		fwrite(&arr[j], sizeof(comp), 1, fw);
	}

	fclose(fw);
	return;
}
void Delete_O_perm() {
	printf("Enter organisation key(for delete data):\n");
	int key_;
	scanf("%d", &key_);
	FILE* f = fopen(Organisation_data_file_name, "rb+");
	comp arr[100];
	fseek(f, 0, SEEK_SET);
	int i;
	for (i = 1; !feof(f); i++) {
		fread(&arr[i], sizeof(comp), 1, f);
		if (arr[i].key == key_) {
			//fread(&arr[i], sizeof(comp), 1, f);
			arr[i].key = 0;
		}
	}
	arr[i - 1].key = -1;
	fclose(f);
	FILE* fw = fopen(Organisation_data_file_name, "wb");
	fseek(fw, 0, SEEK_SET);
	short shiftbool = 0;
	for (int j = 1; arr[j].key != -1; j++) {
		if (arr[j].key == 0) {
			continue;
			shiftbool = 1;
		} if (shiftbool)shift(&arr[j]);
		fwrite(&arr[j], sizeof(comp), 1, fw);
	}

	fclose(fw);
	return;
}
void Delete_O_temp() {
	printf("Enter organisation key(for temp delete data):\n");
	int key_;
	scanf("%d", &key_);
	FILE* f = fopen(Organisation_data_file_name, "rb+");
	comp arr[100];
	fseek(f, 0, SEEK_SET);
	int i;
	for (i = 1; !feof(f); i++) {
		fread(&arr[i], sizeof(comp), 1, f);
		if (arr[i].key == key_) arr[i].deleted = TempDeleted;
	}
	arr[i - 1].key = -1;
	fclose(f);
	FILE* fw = fopen(Organisation_data_file_name, "wb");
	fseek(fw, 0, SEEK_SET);
	for (int j = 1; arr[j].key != -1; j++) {
		fwrite(&arr[j], sizeof(comp), 1, fw);
	}
	fclose(fw);
	return;
}
void Remove_O_temp() {
	printf("Enter organisation key(for remove data):\n");
	int key_;
	scanf("%d", &key_);
	FILE* f = fopen(Organisation_data_file_name, "rb+");
	comp arr[100];
	fseek(f, 0, SEEK_SET);
	int i;
	for (i = 1; !feof(f); i++) {
		fread(&arr[i], sizeof(comp), 1, f);
		if (arr[i].key == key_)arr[i].deleted = DontDeleted;
	}
	arr[i - 1].key = -1;
	fclose(f);
	FILE* fw = fopen(Organisation_data_file_name, "wb");
	fseek(fw, 0, SEEK_SET);
	for (int j = 1; arr[j].key != -1; j++) {
		fwrite(&arr[j], sizeof(comp), 1, fw);
	}
	fclose(fw);
	return;
}
void printCompAll() {
	FILE* f = fopen(Organisation_data_file_name, "rb");
	comp print;
	while (!(feof(f))) {
		fread(&print, sizeof(comp), 1, f);
		printf("Key:%d, Name:%s, Street:%s, Foundation date: %d.%d.%d\n", print.key, print.name, print.street, print.date.day, print.date.month, print.date.year);
		if (print.deleted == TempDeleted) {
			printf("Status: temporary Deleted!");
		}
		else printf("Status: Active!");
		printf("\n\n");

	}
	fclose(f);
	return;
}
void printComp(comp* print) {
	if (!print->deleted)
		printf("Key:%d, Name:%s, Street:%s, Foundation date: %d.%d.%d ", print->key, print->name, print->street, print->date.day, print->date.month, print->date.year);
	else if (print->deleted == TempDeleted)printf("Deleted!\n");
	else printf("Struct not founded!\n");
	return;
}
void Insert_O() {//Organisation
	printf("Enter key, name, address, fundation data(dd mm yyyy):\n");
	FILE* f = fopen(Organisation_data_file_name, "rb+");
	comp temp;
	scanf("%d%40s%40s%d%d%d", &(temp.key), &(temp.name), &(temp.street), &(temp.date.day), &(temp.date.month), &(temp.date.year));
	temp.deleted = DontDeleted;
	fseek(f, 0, SEEK_END);
	fwrite(&temp, sizeof(comp), 1, f);

	FILE* f_s = fopen(Organisation_shift_file_name, "rb+");
	comp_s temp_s;
	temp_s.key = temp.key;
	temp_s.address = ftell(f) / sizeof(comp);
	temp_s.deleted = DontDeleted;
	fseek(f_s, 0, SEEK_END);
	fwrite(&temp_s, sizeof(comp_s), 1, f_s);
	printf("\n");
	printComp(&temp);
	printf("Success Insert!\n");
	fclose(f);
	fclose(f_s);
	return;
}
comp Get_O_r() {
	printf("Enter organisation key(for get data):\n");
	FILE* f_s = fopen(Organisation_shift_file_name, "rb");
	int key_;
	scanf("%d", &key_);
	comp_s temp_s;
	bool flag = 0;
	while (!feof(f_s)) {
		fread(&temp_s, sizeof(comp_s), 1, f_s);
		if (temp_s.key == key_) {
			flag = 1;
			break;
		}
	}
	FILE* f = fopen(Organisation_data_file_name, "rb");
	comp temp;
	fseek(f, sizeof(comp) * (temp_s.address - 1), 1, SEEK_SET);
	fread(&temp, sizeof(comp), 1, f);
	fclose(f);
	fclose(f_s);
	comp temp_d;
	temp_d.name[0] = '-1';
	temp_d.street[0] = '-1';
	temp_d.date.day = -1;
	temp_d.date.month = -1;
	temp_d.date.year = -1;
	temp_d.priority = -1;
	if (!flag) {
		temp_d.deleted = -1;
		return temp_d;
	}
	if (temp.deleted == 2) { return temp_d; }
	else return temp;
}
void Get_O() {
	printf("Enter organisation key(for get data):\n");
	FILE* f_s = fopen(Organisation_shift_file_name, "rb");
	int key_;
	scanf("%d", &key_);
	comp_s temp_s;
	bool flag = 0;
	//int ch = getc(f_s);
	// (ch != EOF)
	while (!feof(f_s)) {
		fread(&temp_s, sizeof(comp_s), 1, f_s);
		if (temp_s.key == key_) {
			flag = 1;
			break;
		}
	}
	FILE* f = fopen(Organisation_data_file_name, "rb");
	comp temp;
	fseek(f, sizeof(comp) * (temp_s.address - 1), 1, SEEK_SET);
	fread(&temp, sizeof(comp), 1, f);
	fclose(f);
	fclose(f_s);
	comp temp_d;
	temp_d.name[0] = '-1';
	temp_d.street[0] = '-1';
	temp_d.date.day = -1;
	temp_d.date.month = -1;
	temp_d.date.year = -1;
	temp_d.priority = -1;
	if (!flag) {
		temp_d.deleted = -1;
		return temp_d;
	}
	if (temp.deleted == 1) { printComp(&temp_d); return; }
	else {
		printComp(&temp); return;
	}
}
void Update_O() {
	printf("Enter organisation key(for update data):\n");
	FILE* f_s = fopen(Organisation_shift_file_name, "rb");
	int key_;
	scanf("%d", &key_);
	comp_s temp_s;
	bool flag = 0;
	while (!feof(f_s)) {
		fread(&temp_s, sizeof(comp_s), 1, f_s);
		if (temp_s.key == key_) {
			flag = 1;
			break;
		}
	}
	if (!flag) {
		printf("Struct don't founded!\n");
		return;
	}
	FILE* f = fopen(Organisation_data_file_name, "rb");
	comp temp;
	comp scan;
	fseek(f, sizeof(comp) * (temp_s.address - 1), 1, SEEK_SET);
	fread(&temp, sizeof(comp), 1, f);
	if (temp.deleted == TempDeleted) {
		printf("Struct temp deleted!");
		return;
	}
	printf("\nEnter name(0 if don't change):");
	scanf("%40s", &(scan.name));
	printf("\nEnter street(0 if don't change):");
	scanf("%40s", &(scan.street));
	printf("\nEnter date(dd mm yyyy)(0 0 0 if don't change):");
	scanf("%d%d%d", &(scan.date.day), &(scan.date.month), &(scan.date.year));
	if (strcmp(scan.name, "0")) {
		strcpy(temp.name, scan.name);
	}
	if (strcmp(scan.street, "0")) {
		strcpy(temp.street, scan.street);
	}
	if (scan.date.day && scan.date.month && scan.date.month) {
		temp.date.day = scan.date.day;
		temp.date.month = scan.date.month;
		temp.date.year = scan.date.year;
	}
	fclose(f);
	//Delete_T_Pointer_f(temp_s.address); //don't need
	FILE* fw = fopen(Organisation_data_file_name, "rb+");
	fseek(fw, sizeof(comp) * (temp_s.address - 1), 1, SEEK_SET);
	fwrite(&temp, sizeof(comp), 1, fw);
	printComp(&temp);
	printf("Success Update!\n");
	fclose(fw);
	return;
}
void temp_total_delete()
{ //not realise
	FILE* f = fopen(Organisation_shift_file_name, "rb");
	comp temp;
	while (1) {
		while (!feof(f)) {
			fread(&temp, sizeof(comp), 1, f);
			if (temp.deleted == TempDeleted)
				Delete_O_perm_key(temp.key);
			temp.deleted = TotalDeleted;
		}
		fclose(f);
	}
	return;
}
void clreaner(){}
//---------------------------------------------Functional

//printCompAll(); //work +
//Insert_O();  //work +
//Update_O(); //work +

/*  comp My = Get_O();	//work +
	printComp(&My); */ //work +

	//Delete_O_temp(); //work +
	//Remove_O_temp(); //work +

	//Delete_O_perm(); //work +

	//temp_total_delete(); //cleaner need realise

int main() {
	printCompAll();
	Get_O();
	return 0;
}

/*
Працівники + файл.
Master(organisation) and Slave file(client) update insert get (can create array)
*/

/*
void GetDataAll(FILE* f, struct company* data[]) {
	fseek(f, 0, SEEK_SET);
	struct company CompTemp;
	int Count = ftell(f) / sizeof(struct company);
	if (Count == 0)printf("FTELL! \n");

	int Arrcount = 0;
	do {
		fread(&CompTemp, sizeof(struct company), 1, f);
		data[Arrcount] = &CompTemp;
		Arrcount++;
	} while (--Count);
	return;
}
void GetData(FILE* f, struct company* data, int shift) {
	if (!shift)
	{
		fseek(f, 0, SEEK_END);
	}
	else {
		fseek(f, sizeof(struct company) * shift, SEEK_SET);
	}
}

//~~~~~~~~~~~~~~INPUT~~~~~~~~~~~~~~~~~~~~~
void inputData(FILE* f, struct company data, int shift) {
	if (!shift)
	{
		fseek(f, 0, SEEK_END);
	}
	else {
		fseek(f, sizeof(struct company) * shift, SEEK_SET);
	}
	fwrite(&data, sizeof(struct company), 1, f);
}

struct My_date {
	int day;
	int month;
	int year;
};

struct company {
	char name[LEN];
	struct My_date date;
	int priority;
};
struct addres {
	int key;
	int add;
};

void Input_adress_last() {
	struct addres tempA;
	FILE* fileAdress = fopen("addres.txt", "w+b");
	if (fileAdress == NULL) {
		printf("Error opening file");
		return;
	}
	fseek(fileAdress, sizeof(struct addres), SEEK_END);
	fscanf(fileAdress, "%d", &tempA.key);
	tempA.key++;
	tempA.add = sizeof(struct company) * tempA.key;
	fwrite(&tempA, sizeof(struct addres), 1, fileAdress);
}

void Add_to_file() {
	int NeedMore = 0;
	struct company comp;
	FILE* fileAdress = fopen("addres.txt", "w+b");
	FILE* fileData = fopen("data.txt", "w+b");
	if (fileAdress == NULL || fileData == NULL) {
		printf("Error opening file");
		return;
	}
	while (1) {
		printf("Enter new Company? [1 or 0]");
		scanf("%d", &NeedMore);
		if (NeedMore == 1) {
			printf("Enter Name - ");
			scanf("%s", &comp.name);
			printf("Enter Data(dd mm yy) - ");
			scanf("%d%d%d", &comp.date.day, &comp.date.month, &comp.date.year);
			printf("This is subsection?[1 or 0] - ");
			scanf("%d", &comp.priority);
			fwrite(&comp, sizeof(struct company), 1, fileData);
			//добавити реліз адрес(функцію)
			//Input_adress_last();
			struct addres tempA;
			//fseek(fileAdress, 0, SEEK_END);
			if (!feof(fileAdress)) {
				tempA.key = 0;
			}
			else fread(&tempA, sizeof(struct addres), 1, fileAdress);
			printf("%d\n", tempA.key);
			tempA.key++;
			tempA.add = sizeof(struct company) * tempA.key;
			fwrite(&tempA, sizeof(struct addres), 1, fileAdress);
		}
		else break;
	}
}

void Show_all() {
	struct addres addres_;
	struct company company_;
	FILE* fileAdress = fopen("addres.txt", "w+b");
	FILE* fileData = fopen("data.txt", "w+b");
	if (fileAdress == NULL || fileData == NULL) {
		printf("Error opening file");
		return;
	}
	printf("Starting Show...\n");
	while (1) {
		fwrite(&addres_, sizeof(struct addres), 1, fileAdress);
		if (addres_.key != NULL) {
			fseek(&fileData, addres_.add, SEEK_CUR);
			fread(&addres_, sizeof(struct addres), 1, fileAdress);
		}
		else break;
	}
	printf("\nClose Show...\n");
}


int main() {
	Add_to_file();
	Show_all();
};


/*
struct stud{
	int year;
	int key;
}out_;

void Input(int count){
	count++;

FILE* iofile = NULL;
unsigned counter = 0;
//int tryy;

iofile = fopen("notes.txt", "w+b");
if (iofile == NULL) {
	printf("Error opening file");
	return;
	//exit(ERROR_OPEN_FILE);
}
struct stud arr[20];
fwrite(&counter, sizeof(int), 1, iofile);
do {
	//printf("Enter new struct? [1 or another]");
	//scanf("%d", &tryy);
	--count;
	if (count>0) {
		printf("Enter year - ");
		scanf("%d", &arr[count].year);
		printf("Enter key - ");
		scanf("%d", &arr[count].key);
		fwrite(&arr[count], sizeof(struct stud), 1, iofile);
		counter++;
	}
	else {
		rewind(iofile);
		fwrite(&counter, sizeof(int), 1, iofile);
		break;
	}

} while (count);
fclose(iofile);
return;
}

void out() {
	FILE* iofile = NULL;
	unsigned counter;
	int i;

	iofile = fopen("notes.txt", "rb");
	if (iofile == NULL) {
		printf("Error opening file");
		return;
	}
	fread(&counter, sizeof(int), 1, iofile);
	for (i = 0; i < counter; i++) {
		fread(&out_, sizeof(out_), 1, iofile);
		printf("Year: %d, key: %d\n Adress: %p\n", out_.year, out_.key, &out_);
	}
	fclose(iofile);
	return;
}

void main() {
	Add_to_file();
	Show_all();
}

if (!strcmp(command, "insert-m"))
{
	struct sStruct supplier;
	supplier.isDeleted = 0;
	printf("Write {name} {status} {city}: ");
	scanf("%s %d %s", &supplier.name, &supplier.status, &supplier.city);
	supplier.spNumber = 0;
	supplier.firstSpKey = -1;
	supplier.key = getNextSKey();

	insertM(&supplier);

	printf("Done! Key = %d\n", supplier.key);
}
void insertM(struct sStruct* supplier)
{
	struct DataInfo dat;

	dat.key = supplier->key;
	dat.position = getSPosition();

	treeInsert(&sKeyTree, dat);

	fseek(sFl, dat.position * sizeof(struct sStruct), SEEK_SET);
	fwrite(supplier, sizeof(struct sStruct), 1, sFl);
}*/