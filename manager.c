#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<time.h>

#define SECONDS_IN_A_DAY 86400

FILE * ExpenseFile;
FILE * LastLoginFile;

char daysOfWeek[7][4] = {"Sun\0","Mon\0", "Tue\0", "Wed\0", "Thu\0", "Fri\0", "Sat\0"};
char Months[12][4] = {"Jan\0", "Feb\0", "Mar\0", "Apr\0", "May\0", "Jun\0",
                      "Jul\0", "Aug\0", "Sep\0","Oct\0", "Nov\0", "Dec\0"};

char CurrMonthFile[200] = "expenses_2024/\0";

struct TimeInfo{
  char day[4];
  int month;
  int date;
  char time[9];
  int year;
};

struct dateStruct{
  int date;
  int month;
  int year;
};

struct dateStruct lastlogin;
struct TimeInfo CurrTime;

int WriteLastLogin(int date);
int readLastLogin();
int FetchTime();
int displayCurrMonth();
int writeCurrMonth();
int nextdate(int * date, int * month, int * year);

int WriteLastLogin(int date){
  
  if (date == 0) date = CurrTime.date;

  LastLoginFile = fopen("expenses_2024/lastlogin.txt", "w");

  if (LastLoginFile == NULL){
    perror("could not open file");
    return 1;
  }
  fprintf(LastLoginFile, "%d/%d/%d", date, CurrTime.month, CurrTime.year);

  fclose(LastLoginFile);
  return 0;  
}


int readLastLogin(){
  LastLoginFile = fopen("expenses_2024/lastlogin.txt", "r");

  if (LastLoginFile == NULL){
    perror("could not open file");
    return 1;
  }

  fscanf(LastLoginFile, "%d/%d/%d", &lastlogin.date, &lastlogin.month, &lastlogin.year);

  return 0;

}

int FetchTime(){
  time_t rawtime;
  struct tm *timeinfo;

  time(&rawtime);
  timeinfo = localtime(&rawtime);
  char * CurrMonthWord;

  strcpy(CurrTime.day,daysOfWeek[timeinfo->tm_wday]);
  CurrTime.month = timeinfo->tm_mon + 1;
  strcpy(CurrMonthWord, Months[timeinfo->tm_mon]);
  CurrTime.date = timeinfo->tm_mday;
  strftime(CurrTime.time, sizeof(CurrTime.time), "%H:%M:%S", timeinfo);
  CurrTime.year = timeinfo->tm_year + 1900;

  printf("%d/%d/%d\n", CurrTime.date, CurrTime.month, CurrTime.year);
  strcat(CurrMonthFile,CurrMonthWord);
  strcat(CurrMonthFile, ".txt");

  //printf("%s\n", CurrMonthFile);

  return 0;
}

int writeCurrMonth(){
  ExpenseFile = fopen(CurrMonthFile, "a");

  char item[100];
  int cost;
  char location[100];
  char nextday;
  

  if(ExpenseFile == NULL) return 2;


  if((lastlogin.month != CurrTime.month) || (lastlogin.year != CurrTime.year)){
    printf("Last Login one or several months ago, please update using custom.\n");
    WriteLastLogin(1);
    return 0;
  }

  if (lastlogin.date == CurrTime.date && lastlogin.month == CurrTime.month 
    && lastlogin.year == CurrTime.year){
    printf("present on current date.\n");
    return 0;
  }

  while(lastlogin.date != CurrTime.date){

    if(nextday == 'n'){
      nextdate(&lastlogin.date, &lastlogin.month, &lastlogin.year);
      continue;
    }

    printf("Currently entering date: %d/%d/%d\n", lastlogin.date, lastlogin.month, lastlogin.year);
    fprintf(ExpenseFile, "%d/%d/%d,", lastlogin.date, lastlogin.month, lastlogin.year);
    printf("Enter Item:");
    scanf("%[^\n]s", item);getchar();
    printf("Enter cost:");
    scanf("%d", &cost);getchar();
    printf("Enter location:");
    scanf("%[^\n]s", location);getchar();
    printf("Next Day(n) or continue(c):");
    scanf("%c", &nextday);getchar();

    fprintf(ExpenseFile, "%s,%d,%s\n", item, cost, location);

  }
  
  WriteLastLogin(0);
  fclose(ExpenseFile);

  return 0;

}


int displayCurrMonth(){
  ExpenseFile = fopen(CurrMonthFile, "r");
  //printf("%s\n", CurrMonthFile);
  
  if (ExpenseFile == NULL) printf("File Not Found");

  char line[1024];
  int dayIter = 0;

  printf("\n-----------------------------\n");
  printf("         Month: %d          \n",CurrTime.month);
  printf("-----------------------------\n");

  printf("\n----------------------------------------------------------------------------\n");
  printf("|%-20s|%-20s|%-20s|%-20s\n","date","item","cost","location");
  printf("----------------------------------------------------------------------------\n");
  while(fgets(line, 1024, ExpenseFile)){
    line[strcspn(line, "\n")] = '\0';

    char *token = strtok(line, ",");

    
    while(token){
      printf("|%-20s", token);
      token = strtok(NULL, ",");
    }printf("\n");
  }
  printf("----------------------------------------------------------------------------\n");

  fclose(ExpenseFile);

  return 0;
}


int nextdate(int * date, int * month, int * year) {
    struct tm start_tm = {0};
    time_t start_time, current_time;

    start_tm.tm_year = *year - 1900;
    start_tm.tm_mon = *month - 1;
    start_tm.tm_mday = *date;
    start_tm.tm_hour = 0;
    start_tm.tm_min = 0;
    start_tm.tm_sec = 0;


    start_time = mktime(&start_tm);


    current_time = start_time;
    current_time += SECONDS_IN_A_DAY;
    struct tm *current_tm = localtime(&current_time);

    *date = current_tm->tm_mday;
    *month = current_tm->tm_mon + 1;
    *year = current_tm->tm_year + 1900;



    return 0;
}

int menu(){ 
  switch(ch){
    
  }

  return 0;

}

int main(){
  // Initialize program by calling FetchTime, readLastLogin
  if (FetchTime() != 0){
    perror("FetchTime");
    return -1;
  }
  if (readLastLogin() != 0){
    perror("readLastLogin");
    return -1;
  }

  writeCurrMonth();
  displayCurrMonth();
  return 0;
}
