#include<iostream>
#include<conio.h>
#include<graphics.h>
#include<windows.h>
#include<string.h>
#include<fstream>
#include<ctype.h>
#include<ctime>
#include<stdio.h>
#include<vector>

using namespace std;

#define BASE_DELAY 200
#define TEMPORARY_FILE "temp.dat"
#define FOR_MANAGER true
#define FOR_CUSTOMER false
#define FOR_ADMIN false
#define REJECTED_IDS "REJECTED_APPLICATION_ID.dat"

void not_matched(char* prompt,int num_try);
void prompter(char* prompt,short value_sent=0);
void gotoxy(short x, short y);
char* converter(string a);
char* fetch();
void cls();
char getc();
void bank_name_display();
void disp_login_portal(char* WINTER_IS_COMING);
bool does_file_exist(char* filename);
bool is_valid_phone(char* phone);
void show_details(struct bio_data ob,bool value_sent=false);

//###################################################################################################

void warner(char* prompt,int delay_time,short value_sent=0)
{
    if(!value_sent)
    gotoxy(0,24);
    else
    if(value_sent==1)
    gotoxy(0,31);
    else
    gotoxy(0,38);
    string temp(prompt);
    int l=temp.length();
    for(int j=1;j<=4;j++)
    {
        cout<<prompt;
        for(int i=0;i<30;i++)
        cout<<" ";
        delay(delay_time);
        for(int i=0;i<l+30;i++)
        cout<<"\b";
        for(int i=0;i<l+30;i++)
        cout<<" ";
        delay(delay_time);
        for(int i=0;i<l+30;i++)
        cout<<"\b";
    }
}

void logger(char* prompt,char* redirect,short value_sent=0)
{
   if(value_sent==1)
   gotoxy(40,30);
   else
   if(value_sent==2)
   gotoxy(40,38);
   else
   gotoxy(40,24);
   string temp(prompt);
   int l=temp.length();
   for(int j=1;j<=4;j++)
    {
        cout<<prompt;
        delay(200);
        for(int i=0;i<l;i++)
        cout<<"\b";
        for(int i=0;i<l;i++)
        cout<<" ";
        delay(200);
        for(int i=0;i<l;i++)
        cout<<"\b";
    }
    warner(redirect,BASE_DELAY,value_sent);
    cls();
}

//####################################################################################################

class Manager;
Manager get_manager_identity();


struct Bank_Details
{
    char* bank_name;
    char* bank_address;
    char* IFSC_code;
    char* branch_code; 
};

class transaction
{
    long double amount;
    char type;
    char recorded_date[12];
    char recorded_time[10];
    char id[30];
    public:
        transaction()
        {
            time_t now=time(0);
            char* datex=ctime(&now);
            
            for(int i=0;i<6;i++)
            recorded_date[i]=datex[4+i];
            
            recorded_date[6]=' ';
            for(int i=7;i<11;i++)
            recorded_date[i]=datex[13+i];

            
            for(int i=0;i<8;i++)
            recorded_time[i]=datex[11+i];
            recorded_date[11]='\0';
            
            recorded_time[8]='\0'; 
        }
        void set(char *ID,long double amt,char ch)
        {
            amount=amt;
            type=ch;  
            strcpy(id,ID); 
        }
        char get_type()
        {
            return type;
        }
        long double get_amount()
        {
            return amount;
        }
        char* get_recorded_date()
        {
            return recorded_date;
        }
        char* get_id()
        {
            return id;
        }
        char* get_recorded_time()
        {
            return recorded_time;
        }
};

class transfer
{
    long double amount;
    char recorded_date[12];
    char recorded_time[10];
    long sender_account_num;
    long reciever_account_num;
    public:
        transfer()
        {
            time_t now=time(0);
            char* datex=ctime(&now);
            
            for(int i=0;i<6;i++)
            recorded_date[i]=datex[4+i];
            
            recorded_date[6]=' ';
            for(int i=7;i<11;i++)
            recorded_date[i]=datex[13+i];

            
            for(int i=0;i<8;i++)
            recorded_time[i]=datex[11+i];
            recorded_date[11]='\0';
            
            recorded_time[8]='\0'; 
        }
        void set(long sender,long reciever,long double amt)
        {
            amount=amt;
            sender_account_num=sender;
            reciever_account_num=reciever;
        }
        long double get_amount()
        {
            return amount;
        }
        char* get_recorded_date()
        {
            return recorded_date;
        }
        char* get_recorded_time()
        {
            return recorded_time;
        }
        long get_sender()
        {
            return sender_account_num;
        }
        long get_reciever()
        {
            return reciever_account_num;
        }
};

Bank_Details bank_details;

struct date
{
    short dd;
    short mm;
    int yyyy;
};

struct bio_data
{
    int age;
    char name[30];
    char address[45];
    char sex;
    struct date dob;
    char phone[13];
    char email[30];
};

enum approval_status{UNCHECKED,APPROVED,DISAPPROVED};

class counter
{
    int idx;
    int count;
    public:
        counter(){}
        counter(int x,int y)
        {
            idx=x;count=y;
        }
        int get_idx(){return idx;}
        int get_count(){return count;}
};

class Generic
{
    protected:
        char user_id[20];
        struct bio_data details;
        char pw[12];
        static char customer_opening_applications_filename[40]; 
    public:
        //virtual bool try_login()=0;
        void set_id(char* id)
        {
            strcpy(user_id,id);
        }
        char* get_id()
        {
            return user_id;
        }
        void set_name(char* n)
        {
            strcpy(details.name,n);
        }
        char* get_name()
        {
            return details.name;
        }  
        void set_pw(char* p)
        {
            strcpy(pw,p);
        }
        char* get_pw()
        {
            return pw;
        } 
        struct bio_data get_bio_data()
        {
            return details;
        } 
        void set_bio_data(struct bio_data bio)
        {
            details=bio;
        } 
        char* get_customer_opening_applications_filename()
        {
            return customer_opening_applications_filename;
        }
};

char Generic::customer_opening_applications_filename[]="CUSTOMER_OPENING_APPLICATIONS.dat";

class System_admin;

class Customer:public Generic
{
    private:
        long account_num;
        static int customer_index;
        static int customer_count;
        char account_type;
        approval_status approved_by_admin;
        approval_status approved_by_manager;
        static char counter_record_filename[30];
        int app_id;
        bool is_accepted;
        long double balance;
        bool closing_request_status;
        short closing_phase;;
    public:
        Customer()
        {
            balance=0;
            closing_phase=0;
            closing_request_status=false;
            approved_by_admin=approved_by_manager=UNCHECKED;
            is_accepted=false;
            if(does_file_exist(counter_record_filename))
            {
                ifstream counter_recorder(counter_record_filename,ios::binary);
                counter temp;
                counter_recorder.read((char*)&temp,sizeof(temp));
                customer_index=temp.get_idx();
                customer_count=temp.get_count();
                counter_recorder.close();
            }
            
        }
        
        void closing_request()
        {
            closing_phase=1;
            closing_request_status=true;
        }
        
        bool get_closing_phase()
        {
            return closing_phase;
        }
        
        void set_closing_phase(short a)
        {
            closing_phase=a;
        }
        
        void closing_request_rejected()
        {
            closing_request_status=false;
            closing_phase=2;
        }
        
        bool get_closing_request_status()
        {
            return closing_request_status;
        }
        
        void set_balance(long double amount)
        {
            balance=amount;
        }
        
        long get_account_num()
        {
            return account_num;
        }
        
        long double get_balance()
        {
            return balance;
        }
        
        void operator =(Customer ob)
        {   
            details=ob.details;
            //customer_index=ob.customer_index;
            //customer_count=ob.customer_count;
            account_type=ob.account_type;
            app_id=ob.app_id;
            approved_by_admin=ob.approved_by_admin;
            approved_by_manager=ob.approved_by_manager;
            account_num=ob.account_num;
        }
        
        void set_initial_status()
        {
            customer_index++;
        }
        
        void accept()
        {
            is_accepted=true;
        }
        
        bool get_is_accepted()
        {
            return is_accepted;
        }
        
        void generate_account_num()
        {
            account_num=137697591+customer_index;
        }
        
        void set_accepted_status()
        {
            customer_count++;
        }
        
        void set_account_type(char ch)
        {
            account_type=ch;
        }
        
        char get_account_type()
        {
            return account_type;
        }
        
        void generate_id()
        {
           strcpy(user_id,"M_BRV_IDX::");
           char temp[2];
           temp[0]=65+customer_index;
           temp[1]='\0';
           strcat(user_id,temp);
           temp[0]=48+customer_index;
           strcat(user_id,temp);
        }
        
        void generate_app_id()
        {
          app_id=1233+customer_index;  
        }
        
        int get_app_id()
        {
            return app_id;
        }
        void new_customer_application()
        {
            Customer ob=*this;
            ofstream customer_record_writer(customer_opening_applications_filename,ios::binary|ios::app);
            customer_record_writer.write((char*)&ob,sizeof(ob));
            customer_record_writer.close();
        }
        
        approval_status get_admin_approval()
        {
            return approved_by_admin;
        }
        
        char* get_admin_status()
        {
            if(approved_by_admin==UNCHECKED)
            return "UNCHECKED";
            else
            if(approved_by_admin==APPROVED)
            return "APPROVED";
            else
            return "DISAPPROVED";
        }
        
        approval_status get_manager_approval()
        {
            return approved_by_manager;
        }
        
        char* get_manager_status()
        {
            if(approved_by_manager==UNCHECKED)
            return "UNCHECKED";
            else
            if(approved_by_manager==APPROVED)
            return "APPROVED";
            else
            return "DISAPPROVED";
        }
        
        void set_admin_approval_status(approval_status stat)
        {
            approved_by_admin=stat;
        }
        
        void set_manager_approval_status(approval_status stat)
        {
            approved_by_manager=stat;
        }
        
        ~Customer()
        {
            ofstream counter_recorder(counter_record_filename,ios::binary);
            counter_recorder.write((char*)(new counter(customer_index,customer_count)),sizeof(counter));
            counter_recorder.close();
        }
        
        friend void customer_deleted();     
        
};

int Customer::customer_index=0;
int Customer::customer_count=0;
char Customer::counter_record_filename[]="COUNTER_RECORD_FILENAME.txt";

void customer_deleted()
{
    Customer::customer_count--;
}

class Manager:public Generic
{
    private:
        //DERIVED::static char* customer_opening_applications_filename;
        static char money_transfer_applications_filename[60];
        static char customer_closing_applications_filename[60];
        static char registered_customers_filename[60];
        static char transactions_filename[30];
        bool status_updated;
    public:
        
        Manager()
        { 
            status_updated=false;        
        }
        
        bool set_status_update(bool boolean_var)
        {
            status_updated=boolean_var;
        }
        
        bool get_status_update()
        {
            return status_updated;
        }
        
        char* get_registered_customers_filename()
        {
            return registered_customers_filename;
        }
        
        char* get_transfer_applications_filename()
        {
            return money_transfer_applications_filename;
        }
        
        char* get_transactions_filename()
        {
            return transactions_filename;
        }
        
        char* get_customer_closing_applications_filename()
        {
            return customer_closing_applications_filename; 
        }
        
        void operator = (Manager ob)
        {
            details=ob.details;
            strcpy(user_id,ob.user_id);
            strcpy(pw,ob.pw);
        }
        
        void delete_customer(char *id)                                            //SYSTEM ADMIN MEMBER FUNCTION
        {
            ifstream customer_accounts_eraser(registered_customers_filename,ios::binary);
            ofstream temp(TEMPORARY_FILE,ios::binary);
            Customer ob;
            while(customer_accounts_eraser.read((char*)&ob,sizeof(ob)))
            {   
                if(strcmp(ob.get_id(),id)!=0)
                    temp.write((char*)&ob,sizeof(ob));
            }
            customer_accounts_eraser.close();
            temp.close();
            remove(registered_customers_filename);
            rename(TEMPORARY_FILE,registered_customers_filename);
            customer_deleted();
        }
        
        void new_customer_registry(Customer ob)
        {
            ofstream customer_record_writer(registered_customers_filename,ios::app|ios::binary);
            ob.generate_id();
            ob.generate_account_num();
            ob.set_accepted_status();
            customer_record_writer.write((char*)&ob,sizeof(ob));
            customer_record_writer.close();
        }
        
        bool try_login()
        {
            int FLAG=0,FLAG_2=0;
            int login_tries=5;
            char* temp;
            char ch;
            int temp_idx=0;
            cls();
            
            Manager tempo=get_manager_identity();
            strcpy(user_id,tempo.user_id);
            strcpy(pw,tempo.pw);
            strcpy(details.name,tempo.details.name);
            
            while(login_tries)
            {
                //cls();
                bank_name_display();
                disp_login_portal("MANAGER");
                gotoxy(24,8);
                cout<<"ENTER USER ID/NAME>>>>>    ";
                temp=fetch();
                if(strcmp(temp,user_id)==0)
                {
                    FLAG_2=1;
                    //warner("ID MATCH FOUND!!",BASE_DELAY);
                    break;
                }
                else
                if(stricmp(temp,details.name)==0)
                {
                    FLAG_2=2;
                    //warner("NAME MATCH FOUND!!",BASE_DELAY);
                    break;
                }
                login_tries--;
                if(login_tries)
                not_matched("USER-ID/NAME NOT FOUND",login_tries);
            }
            
            if(!login_tries)
                return false;
            else
            {
                login_tries=5;
                
                while(login_tries)
                {   
                    cls();
                    FLAG=0;
                    strcpy(temp,"\0");
                    temp_idx=0;
                    bank_name_display();
                    disp_login_portal("MANAGER");
                    gotoxy(24,8);
                    cout<<"ENTER USER ID/NAME>>>>>    ";
                    if(FLAG_2==1)
                    cout<<user_id;
                    else
                    cout<<details.name;
                    gotoxy(24,12);
                    cout<<"ENTER PASSWORD>>>>         ";
                    prompter("PRESS SPACE AT ANY MOMENT TO SHOW/HIDE YOUR PASSWORD");
                    gotoxy(24,12);
                    cout<<"ENTER PASSWORD>>>>         ";
                    while(1)
                    {
                        ch=getche();
                        if(ch==8)
                        {
                            if(!temp_idx)
                            continue;
                            cout<<" \b";
                            temp_idx--;
                            temp[temp_idx]='\0';
                        }
                        else
                        if(ch==13)
                        break;
                        else
                        if(ch==32)
                        {
                            FLAG=!FLAG;
                            gotoxy(24,12);
                            cout<<"ENTER PASSWORD>>>>         ";
                            for(int z=1;z<=15;z++)
                            cout<<" ";
                            for(int z=1;z<=15;z++)
                            cout<<"\b";
                            if(FLAG)
                                cout<<temp;
                            else
                                for(int z=0;temp[z]!='\0';z++)
                                    cout<<"*";  
                        }
                        else
                        if(isalpha(ch) || isdigit(ch) || ispunct(ch))
                        {
                            cout<<"\b \b";
                            if(temp_idx==10)
                            {
                                warner("PASSWORD LENGTH EXCEEDED!!",BASE_DELAY);
                                fflush(stdin);
                                gotoxy(24,12);
                                cout<<"ENTER PASSWORD>>>>         ";
                                for(int z=1;z<=15;z++)
                                cout<<" ";
                                for(int z=1;z<=15;z++)
                                cout<<"\b";
                                if(FLAG)
                                    cout<<temp;
                                else
                                    for(int z=0;temp[z]!='\0';z++)
                                        cout<<"*"; 
                                continue;
                            }
                            if(FLAG)
                            cout<<ch;
                            else
                            cout<<"*";
                            temp[temp_idx]=ch;
                            temp[temp_idx+1]='\0';
                            temp_idx++;
                        }
                    }
                    
                    if(strcmp(temp,pw)==0)
                    {
                        //warner("PASSWORD MATCH FOUND!!",BASE_DELAY);
                        return true;
                    } 
   
                    login_tries--;
                    if(login_tries)
                    not_matched("PASSWORD NOT MATCHED!!",login_tries);
                }
                return false;
            }
        }
        
      friend void customer_approval(bool);  
      friend bool try_customer_login(char* &ID);
      friend void account_closing_approval();
      friend void transfer_approval();
};

char Manager::money_transfer_applications_filename[]="MONEY_TRANSFER_APPLICATIONS.dat";
char Manager::customer_closing_applications_filename[]="CUSTOMER_CLOSING_APPLICATIONS.dat";
char Manager::registered_customers_filename[]="REGISTERED_CUSTOMERS.dat";
char Manager::transactions_filename[]="CUSTOMER_TRANSACTIONS.dat";

bool try_customer_login(char* &ID)
{
    bool FLAG=false;
    int FLAG_2=0;
    int login_tries=5;
    char temp[30];
    char ch;
    bool FLAG_3=true;
    int temp_idx=0;
    cls();
    
    vector<char*> pass;
    vector<char*> id;
    vector<char*> nm;
    int found_position;
    
    while(login_tries && FLAG_3)
    {
        cls();
        bank_name_display();
        disp_login_portal("CUSTOMER");
        gotoxy(24,8);
        cout<<"ENTER USER ID/NAME>>>>>    ";
        gets(temp);
        fflush(stdin);
        
        if(does_file_exist(Manager::registered_customers_filename))
        {   
            ifstream customer_list(Manager::registered_customers_filename,ios::binary);
            Customer ob;
            while(customer_list.read((char*)&ob,sizeof(ob)))
            {
                pass.push_back(ob.get_pw());
                id.push_back(ob.get_id());
                nm.push_back(ob.get_name());
            }
            customer_list.close();
        }
        else
        {
            logger("NO CUSTOMERS REGISTERED","REDIRECTING TO PREVIOUS PAGE");
            return false; 
        }
        
        int customer_num=pass.size();

        for(int i=1;i<=customer_num;i++)
        {   
            if(strcmp(temp,id[i-1])==0)
            {
            FLAG_2=1;
            found_position=i-1;
            //warner("ID MATCH FOUND!!",BASE_DELAY);
            FLAG_3=false;
            break;
            }
            else
            if(stricmp(temp,nm[i-1])==0)
            {
            FLAG_2=2;
            found_position=i-1;
            //warner("NAME MATCH FOUND!!",BASE_DELAY);
            FLAG_3=false;
            break;
            }
            login_tries--;
            if(login_tries)
            not_matched("USER-ID/NAME NOT FOUND",login_tries);
            else break;
        }
    }
    
    if(!login_tries)
        return false;
    else
    {
        login_tries=5;
        while(login_tries)
        {   
            cls();
            FLAG=false;
            strcpy(temp,"\0");
            temp_idx=0;
            bank_name_display();
            disp_login_portal("CUSTOMER");
            gotoxy(24,8);
            cout<<"ENTER USER ID/NAME>>>>>    ";
            if(FLAG_2==1)
            cout<<id[found_position];
            else
            cout<<nm[found_position];
            gotoxy(24,12);
            cout<<"ENTER PASSWORD>>>>         ";
            prompter("PRESS SPACE AT ANY MOMENT TO SHOW/HIDE YOUR PASSWORD");
            gotoxy(24,12);
            cout<<"ENTER PASSWORD>>>>         ";
            while(1)
            {
                ch=getche();
                if(ch==8)
                {
                    if(!temp_idx)
                    continue;
                    cout<<" \b";
                    temp_idx--;
                    temp[temp_idx]='\0';
                }
                else
                if(ch==13)
                break;
                else
                if(ch==32)
                {
                    FLAG=!FLAG;
                    gotoxy(24,12);
                    cout<<"ENTER PASSWORD>>>>         ";
                    for(int z=1;z<=15;z++)
                    cout<<" ";
                    for(int z=1;z<=15;z++)
                    cout<<"\b";
                    if(FLAG)
                        cout<<temp;
                    else
                        for(int z=0;temp[z]!='\0';z++)
                            cout<<"*";  
                }
                else
                if(isalpha(ch) || isdigit(ch) || ispunct(ch))
                {
                    cout<<"\b \b";
                    if(temp_idx==10)
                    {
                        warner("PASSWORD LENGTH EXCEEDED!!",BASE_DELAY);
                        fflush(stdin);
                        gotoxy(24,12);
                        cout<<"ENTER PASSWORD>>>>         ";
                        for(int z=1;z<=15;z++)
                        cout<<" ";
                        for(int z=1;z<=15;z++)
                        cout<<"\b";
                        if(FLAG)
                            cout<<temp;
                        else
                            for(int z=0;temp[z]!='\0';z++)
                                cout<<"*"; 
                        continue;
                    }
                    if(FLAG)
                    cout<<ch;
                    else
                    cout<<"*";
                    temp[temp_idx]=ch;
                    temp[temp_idx+1]='\0';
                    temp_idx++;
                }
            }
            
            if(strcmp(temp,pass[found_position])==0)
            {
                //warner("PASSWORD MATCH FOUND!!",BASE_DELAY);
                strcpy(ID,id[found_position]);
                return true;
            } 

            login_tries--;
            if(login_tries)
            not_matched("PASSWORD NOT MATCHED!!",login_tries);
        }
        return false;
    }
}
        

class System_admin :public Generic
{
    private:
        static char manager_filename[60];
        static char manager_accounts_history[60];
        //DERIVED::static char* customer_opening_applications_filename; 
    public:
        friend Manager get_manager_identity();
        char* get_manager_filename()
        {
            return manager_filename;
        }
        
        void new_manager(Manager ob)
        {
            ofstream manager_record_writer(manager_filename,ios::binary);
            manager_record_writer.write((char*)&ob,sizeof(ob));
            manager_record_writer.close();
            manager_record_writer.open(manager_accounts_history,ios::app);
            manager_record_writer.write((char*)&ob,sizeof(ob));
            manager_record_writer.close();
        }
        
        void delete_manager()
        {
            remove(manager_filename);
        }  
        
        Manager get_current_manager()
        {
            ifstream manager_record_reader(manager_filename,ios::binary);
            Manager ob;
            manager_record_reader.read((char*)&ob,sizeof(ob));
            manager_record_reader.close();
            return ob;
        }
                  
        bool try_login()
        {
            int FLAG=0,FLAG_2=0;
            int login_tries=5;
            char* temp;
            char ch;
            int temp_idx=0;
            cls();
            while(login_tries)
            {
                //cls();
                bank_name_display();
                disp_login_portal("SYSTEM_ADMIN");
                gotoxy(24,8);
                cout<<"ENTER USER ID/NAME>>>>>    ";
                temp=fetch();
                if(strcmp(temp,user_id)==0)
                {
                    FLAG_2=1;
                    //warner("ID MATCH FOUND!!",BASE_DELAY);
                    break;
                }
                else
                if(stricmp(temp,details.name)==0)
                {
                    FLAG_2=2;
                    //warner("NAME MATCH FOUND!!",BASE_DELAY);
                    break;
                }
                login_tries--;
                if(login_tries)
                not_matched("USER-ID/NAME NOT FOUND",login_tries);
            }
            
            if(!login_tries)
                return false;
            else
            {
                login_tries=5;
                
                while(login_tries)
                {   
                    AGAIN:
                    cls();
                    FLAG=0;
                    strcpy(temp,"\0");
                    temp_idx=0;
                    bank_name_display();
                    disp_login_portal("SYSTEM_ADMIN");
                    gotoxy(24,8);
                    cout<<"ENTER USER ID/NAME>>>>>    ";
                    if(FLAG_2==1)
                    cout<<user_id;
                    else
                    cout<<details.name;
                    gotoxy(24,12);
                    cout<<"ENTER PASSWORD>>>>         ";
                    prompter("PRESS SPACE AT ANY MOMENT TO SHOW/HIDE YOUR PASSWORD");
                    gotoxy(24,12);
                    cout<<"ENTER PASSWORD>>>>         ";
                    while(1)
                    {
                        ch=getche();
                        if(ch==8)
                        {
                            if(!temp_idx)
                            continue;
                            cout<<" \b";
                            temp_idx--;
                            temp[temp_idx]='\0';
                        }
                        else
                        if(ch==13)
                        break;
                        else
                        if(ch==32)
                        {
                            FLAG=!FLAG;
                            gotoxy(24,12);
                            cout<<"ENTER PASSWORD>>>>         ";
                            for(int z=1;z<=15;z++)
                            cout<<" ";
                            for(int z=1;z<=15;z++)
                            cout<<"\b";
                            if(FLAG)
                                cout<<temp;
                            else
                                for(int z=0;temp[z]!='\0';z++)
                                    cout<<"*";  
                        }
                        else
                        if(isalpha(ch) || isdigit(ch) || ispunct(ch))
                        {
                            cout<<"\b \b";
                            if(temp_idx==10)
                            {
                                warner("PASSWORD LENGTH EXCEEDED!!",BASE_DELAY);
                                gotoxy(24,12);
                                cout<<"ENTER PASSWORD>>>>         ";
                                for(int z=1;z<=15;z++)
                                cout<<" ";
                                for(int z=1;z<=15;z++)
                                cout<<"\b";
                                if(FLAG)
                                cout<<temp;
                                else
                                    for(int z=0;temp[z]!='\0';z++)
                                        cout<<"*";
                                continue;
                            }
                            if(FLAG)
                            cout<<ch;
                            else
                            cout<<"*";
                            temp[temp_idx]=ch;
                            temp[temp_idx+1]='\0';
                            temp_idx++;
                        }
                    }
                    
                    if(strcmp(temp,pw)==0)
                    {
                        //warner("PASSWORD MATCH FOUND!!",BASE_DELAY);
                        return true;
                    } 
   
                    login_tries--;
                    if(login_tries)
                    not_matched("PASSWORD NOT MATCHED!!",login_tries);
                }
                return false;
            }
        }
        friend void customer_approval(bool);
};

char System_admin::manager_filename[]="CURRENT_MANAGER_RECORD.dat";
char System_admin::manager_accounts_history[]="MANAGER_ACCOUNTS_HISTORY.dat";

Manager get_manager_identity()
{
    ifstream manager_record_reader(System_admin::manager_filename,ios::binary);
    Manager ob;
    manager_record_reader.read((char*)&ob,sizeof(ob));
    ob.get_name();
    ob.get_id();
    ob.get_pw();
    return ob;
} 

bool is_empty(ifstream& filename)
{
    return filename.peek()==ifstream::traits_type::eof();
}

void customer_approval(bool boolean_var)
{
    ifstream customer_applicants(Manager::customer_opening_applications_filename,ios::binary);
    if(is_empty(customer_applicants))
    {
        cls();
        bank_name_display();
        logger("EMPTY LIST...NO RECORDS FOUND!!","REDIRECTING TO MENU PAGE...");
        customer_applicants.close();
        return;
    }
    
    vector<Customer> customer_list;
    Customer ob;
    
    while(customer_applicants.read((char*)&ob,sizeof(ob)))
    customer_list.push_back(ob);
    
    customer_applicants.close();
    int customer_num=customer_list.size();
    
    int customer_index[100];
    short customer_index_loc=-1;
    
    while(1)
    {
        cls();
        bank_name_display();
        int index=1;
        
        cout<<endl<<endl;
    
        cout<<"           SL.        APPLICATION ID        ACCOUNT TYPE"<<endl;
        cout<<"          ----       ----------------      ----------------"<<endl;        
    
        for(int i=1;i<=customer_num;i++)
        {
            if(boolean_var==FOR_MANAGER)
            {
                if(customer_list[i-1].get_admin_approval()==APPROVED)
                {
                    customer_index_loc++;
                    customer_index[customer_index_loc]=i-1;
                    
                    cout<<"           "<<index++<<"              ";
                    cout<<customer_list[i-1].get_app_id()<<"                 ";
                    char ch=customer_list[i-1].get_account_type();
                    if(ch=='C' || ch=='c')
                    cout<<"CURRENT";
                    else
                    if(ch=='S' ||ch=='s')
                    cout<<"STUDENT";
                    else
                    cout<<"SAVINGS";
                    cout<<endl<<endl;
                }
            }
            else
            {
                cout<<"           "<<index++<<"              ";
                cout<<customer_list[i-1].get_app_id()<<"                 ";
                char ch=customer_list[i-1].get_account_type();
                if(ch=='C' || ch=='c')
                cout<<"CURRENT";
                else
                if(ch=='S' ||ch=='s')
                cout<<"STUDENT";
                else
                cout<<"SAVINGS";
                cout<<endl<<endl;
            }
        }
        
        if(boolean_var==FOR_MANAGER && customer_index_loc==-1)
        {
            cls();
            bank_name_display();
            logger("EMPTY LIST...NO RECORDS FOUND!!","REDIRECTING TO MENU PAGE...");
            return;
        }
       
        prompter("PLEASE SELECT APPLICATION ID...",2);
        prompter("\nPRESS ESCAPE TO CONFIRM AND GO BACK TO PREVIOUS SCREEN",2); 
        char ch=0;
        gotoxy(60,6);
        cout<<"<<<<<<<";
        int sel=0;
        while(ch!=13)
        {
            ch=getc();
            if(ch==27)
            {
                prompter("                                       ",2);
                prompter("\n                                                          ",2);
                
                ofstream refreshed_customer_applicants(Manager::customer_opening_applications_filename,ios::binary);
                
                Customer ob;
                Manager busy_manager;
                ofstream rejected_ids(REJECTED_IDS,ios::binary|ios::app);
                for(int i=1;i<=customer_num;i++)
                {
                    ob=customer_list[i-1];
                    if(ob.get_manager_approval()==APPROVED && ob.get_admin_approval()==APPROVED)  
                    busy_manager.new_customer_registry(ob);
                    else
                    if(ob.get_admin_approval()==DISAPPROVED || (ob.get_admin_approval()==APPROVED && ob.get_manager_approval()==DISAPPROVED )) 
                    rejected_ids.write((char*)&ob,sizeof(ob));
                    else
                    refreshed_customer_applicants.write((char*)&ob,sizeof(ob));
                    
                }
                
                refreshed_customer_applicants.close();
                rejected_ids.close();
                logger("CONFIRMING APROVALS!!","REDIRECTING TO MENU PAGE...",1);
                return;
            }
            else 
            if(ch==80)
            sel=(sel+1)%customer_num;
            else
            if(ch==72)
            sel=(sel+customer_num-1)%customer_num;
            cout<<"\b\b\b\b\b\b\b       ";
            gotoxy(60,6+sel*2);
            cout<<"<<<<<<<";
        }
        
        if(boolean_var==FOR_MANAGER)
        sel=customer_index[sel];
        
        show_details(customer_list[sel].get_bio_data(),true);
        
        gotoxy(13,17);
        cout<<"ACCOUNT TYPE: ";
        char ch2=customer_list[sel].get_account_type();
        if(ch2=='C' || ch2=='c')
        cout<<"CURRENT";
        else
        if(ch2=='S' ||ch2=='s')
        cout<<"STUDENT";
        else
        cout<<"SAVINGS";
        
        gotoxy(13,19); 
        cout<<"CURRENT STATUS:-";
        gotoxy(13,21);
        cout<<"        SYSTEM_ADMIN: "<<customer_list[sel].get_admin_status()<<endl<<endl;
        gotoxy(13,23);
        cout<<"        MANAGER: "<<customer_list[sel].get_manager_status();
        
        while(1)
        { 
            prompter("Press:-\n   Y-->APPROVE\n   N-->DISAPPROVE",1);
            char ch=getc();
            prompter("       \n              \n                 ",1);
            if(ch=='Y' || ch=='y')
            {
                if(boolean_var==FOR_MANAGER)
                customer_list[sel].set_manager_approval_status(APPROVED);
                else
                customer_list[sel].set_admin_approval_status(APPROVED);
                logger("CUSTOMER APPROVED!!","REDIRECTING TO PREVIOUS PAGE...",1);
                break;
            }
            else
            if(ch=='N' || ch== 'n')
            {
                if(boolean_var==FOR_MANAGER)
                customer_list[sel].set_manager_approval_status(DISAPPROVED);
                else
                customer_list[sel].set_admin_approval_status(DISAPPROVED);
                logger("CUSTOMER DISAPPROVED!!","REDIRECTING TO PREVIOUS PAGE...",1);
                break;
            }
            else
            warner("INVALID INPUT!!",BASE_DELAY);
        }
        
    }
}

void account_closing_approval()
{
    //cout<<does_file_exist(Manager::customer_closing_applications_filename);getc();
    ifstream closing_applicants(Manager::customer_closing_applications_filename,ios::binary);
    if(is_empty(closing_applicants))
    {
        cls();
        bank_name_display();
        logger("EMPTY LIST...NO RECORDS FOUND!!","REDIRECTING TO MENU PAGE...");
        closing_applicants.close();
        return;
    }
    
    vector<Customer> closing_list;
    vector<char*> id_list;
    Customer ob;
    
    while(closing_applicants.read((char*)&ob,sizeof(ob)))
    closing_list.push_back(ob);
    
    closing_applicants.close();
    int closing_num=closing_list.size();
    
    int closing_index[100];
    short closing_index_loc=-1;
    
    while(1)
    {
        cls();
        bank_name_display();
        
        
        cout<<endl<<endl;
    
        cout<<"           SL.           USER ID            ACCOUNT TYPE"<<endl;
        cout<<"          ----       ----------------      ----------------"<<endl;        
    
        for(int i=1;i<=closing_num;i++)
        {
            cout<<"           "<<i<<"           ";
            cout<<closing_list[i-1].get_id()<<"          ";
            char ch=closing_list[i-1].get_account_type();
            if(ch=='C' || ch=='c')
            cout<<"CURRENT";
            else
            if(ch=='S' ||ch=='s')
            cout<<"STUDENT";
            else
            cout<<"SAVINGS";
            cout<<endl<<endl;
        }
       
        prompter("PLEASE SELECT USER ID...",2);
        prompter("\nPRESS ESCAPE TO CONFIRM AND GO BACK TO PREVIOUS SCREEN",2); 
        char ch=0;
        gotoxy(60,6);
        cout<<"<<<<<<<";
        int sel=0;
        while(ch!=13)
        {
            ch=getc();
            if(ch==27)
            {
                prompter("                                       ",2);
                prompter("\n                                                          ",2);
                
                Manager manager;
                for(int i=0;i<id_list.size();i++)
                manager.delete_customer(id_list[i]);
                
                logger("CONFIRMING APROVALS!!","REDIRECTING TO MENU PAGE...",1);
                return;
            }
            else 
            if(ch==80)
            sel=(sel+1)%closing_num;
            else
            if(ch==72)
            sel=(sel+closing_num-1)%closing_num;
            cout<<"\b\b\b\b\b\b\b       ";
            gotoxy(60,6+sel*2);
            cout<<"<<<<<<<";
        }
        
        
        show_details(closing_list[sel].get_bio_data(),true);
        
        gotoxy(13,17);
        cout<<"ACCOUNT TYPE: ";
        char ch2=closing_list[sel].get_account_type();
        if(ch2=='C' || ch2=='c')
        cout<<"CURRENT";
        else
        if(ch2=='S' ||ch2=='s')
        cout<<"STUDENT";
        else
        cout<<"SAVINGS";
        
        gotoxy(13,19); 
        cout<<"CURRENT BALANCE: "<<ob.get_balance();
        
        while(1)
        { 
            prompter("Press:-\n   Y-->APPROVE\n   N-->DISAPPROVE",1);
            char ch=getc();
            prompter("       \n              \n                 ",1);
            if(ch=='Y' || ch=='y')
            {
                id_list.push_back(closing_list[sel].get_id());
                logger("ACCOUNT CLOSING APPROVED!!","REDIRECTING TO PREVIOUS PAGE...",1);
                break;
            }
            else
            if(ch=='N' || ch== 'n')
            {
                logger("ACCOUNT CLOSING DISAPPROVED!!","REDIRECTING TO PREVIOUS PAGE...",1);
                break;
            }
            else
            warner("INVALID INPUT!!",BASE_DELAY);
        }
        
    }
}

/*void transfer_approval()
{
    ifstream transfer_applicants(Manager::money_transfer_applications_filename,ios::binary);
    if(is_empty(transfer_applicants))
    {
        cls();
        bank_name_display();
        logger("EMPTY LIST...NO RECORDS FOUND!!","REDIRECTING TO MENU PAGE...");
        transfer_applicants.close();
        return;
    }
    
    vector<transfer> transfer_list;
    vector<char*> id_list;
    Customer ob;
    
    while(transfer_applicants.read((char*)&ob,sizeof(ob)))
    transfer_list.push_back(ob);
    
    transfer_applicants.close();
    int transfer_num=transfer_list.size();
    
    int transfer_index[100];
    short transfer_index_loc=-1;
    
    while(1)
    {
        cls();
        bank_name_display();
        int index=1;
        
        cout<<endl<<endl;
    
        cout<<"   SL.      SENDER ACCOUNT     RECIEVER ACCOUNT  AMOUNT      DATE        TIME"<<endl;
        cout<<"  ----     ----------------    ----------------  ------  -----------   -------"<<endl;        
    
        for(int i=1;i<=transfer_num;i++)
        {
            cout<<"   "<<index++<<"   ";
            cout<<transfer_list[i-1].get_sender_account_num()<<"  ";
            cout<<transfer_list[i-1].get_reciever_account_num()<<"  ";
            cout<<transfer_list[i-1].get_amount()<<"  ";
            cout<<transfer_list[i-1].get_recorded_date();<<"  ";
            cout<<transfer_list[i-1].get_recorded_time();<<"  ";
            
            cout<<endl<<endl;
        }
        
       
        prompter("PLEASE SELECT...",2);
        prompter("\nPRESS ESCAPE TO CONFIRM AND GO BACK TO PREVIOUS SCREEN",2); 
        char ch=0;
        gotoxy(60,6);
        cout<<"<<<<<<<";
        int sel=0;
        while(ch!=13)
        {
            ch=getc();
            if(ch==27)
            {
                prompter("                                       ",2);
                prompter("\n                                                          ",2);
                
                Manager manager;
                for(int i=0;i<id_list.size();i++)
                manager.delete_customer(id_list[i]);
                
                logger("CONFIRMING APROVALS!!","REDIRECTING TO MENU PAGE...",1);
                return;
            }
            else 
            if(ch==80)
            sel=(sel+1)%transfer_num;
            else
            if(ch==72)
            sel=(sel+transfer_num-1)%transfer_num;
            cout<<"\b\b\b\b\b\b\b       ";
            gotoxy(60,6+sel*2);
            cout<<"<<<<<<<";
        }
        
        
        show_details(transfer_list[sel].get_bio_data(),true);
        
        gotoxy(13,17);
        cout<<"ACCOUNT TYPE: ";
        char ch2=transfer_list[sel].get_account_type();
        if(ch2=='C' || ch2=='c')
        cout<<"CURRENT";
        else
        if(ch2=='S' ||ch2=='s')
        cout<<"STUDENT";
        else
        cout<<"SAVINGS";
        
        gotoxy(13,19); 
        cout<<"CURRENT BALANCE: "<<ob.get_balance();
        
        while(1)
        { 
            prompter("Press:-\n   Y-->APPROVE\n   N-->DISAPPROVE",1);
            char ch=getc();
            prompter("       \n              \n                 ",1);
            if(ch=='Y' || ch=='y')
            {
                id_list.push_back(transfer_list[sel].get_id());
                logger("ACCOUNT CLOSING APPROVED!!","REDIRECTING TO PREVIOUS PAGE...",1);
                break;
            }
            else
            if(ch=='N' || ch== 'n')
            {
                logger("ACCOUNT CLOSING DISAPPROVED!!","REDIRECTING TO PREVIOUS PAGE...",1);
                break;
            }
            else
            warner("INVALID INPUT!!",BASE_DELAY);
        }
        
    }
}*/

/*############################################################################################################*/


void gotoxy(short x, short y)
{
    COORD pos = {x, y};
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

char getc()
{
    char ch;
    ch=getche();
    cout<<"\b ";
    return ch;
}

char* converter(string a)
{
    char* x;
    x=&a[0];
    return x;
}

char* fetch()
{
    string a;
    getline(cin,a);
    return converter(a);
}

void cls()
{
    system("cls");
}

bool is_valid_phone(char* phone)
{
    if((phone[0]!='+' && strlen(phone)!=10) && strlen(phone)!=13)
    return false;
    for(int j=0;phone[j]!='\0';j++)
    if(phone[j]<48 || phone[j]>57)
    return false;
    return true;
}

void prompter(char* prompt,short value)
{
    if(!value)
    gotoxy(0,24);
    else
    if(value==1)
    gotoxy(0,31);
    else
    gotoxy(0,37);
    string temp(prompt);
    cout<<prompt;
}

void not_matched(char* prompt,int num_try)
{
    warner(prompt,BASE_DELAY);
    gotoxy(40,27);
    cout<<prompt;
    gotoxy(40,28);
    cout<<"NO. OF ATTEMPTS MADE: "<<5-num_try;
    gotoxy(40,29);
    cout<<"NO. OF ATTEMPTS LEFT: "<<num_try;
    getche();
    cout<<"\b ";
    cls();
} 

void set_bank_details()
{
    bank_details.bank_name="The Iron Bank Of Braavos";
    bank_details.bank_address="Iron Banks,\n                       West Valyria,\n                       Braavos,Westeros";
    bank_details.IFSC_code="IFSC_SLTBMGTF_9436193664";
    bank_details.branch_code="TIBOB_AZOR_AHAI#327";
}

void set_admin(System_admin& admin)
{
   admin.set_name("ADMIN");
   admin.set_id("AZOR_AHAI");
   admin.set_pw("password");
}

bool does_file_exist(char* filename)
{
    ifstream file(filename,ios::binary);
    bool boolean_var=file;
    file.close();
    return boolean_var;
}

/*###########################################################################################################*/

void bank_name_display()
{
    cout<<"<<<<<<<<<<<<<<<<<<<<<<<<<THE  IRON  BANK  OF  BRAAVOS>>>>>>>>>>>>>>>>>>>>>>>>>"<<endl;
    cout<<"                         ----------------------------"<<endl;
}

char* password_changer()
{
    while(1)
    {
        cls();
        bank_name_display();
        static char temp1[12],temp2[12];
        int temp_idx=0;
        bool VISIBLE=false;
        char ch;
        prompter("PRESS SPACEBAR TO HIDE/SHOW THE PASSWORD");
        gotoxy(10,8);
        cout<<"ENTER NEW PASSWORD: ";
        while(1)
        {
            ch=getche();
            if(ch==32)
            {
                VISIBLE=!VISIBLE;
                gotoxy(10,8);
                cout<<"ENTER NEW PASSWORD: ";
                for(int z=1;z<=15;z++)
                cout<<" ";
                for(int z=1;z<=15;z++)
                cout<<"\b";
                if(VISIBLE)
                    cout<<temp1;
                else
                    for(int z=0;temp1[z]!='\0';z++)
                        cout<<"*";  
            }
            else
            if(ch==8)
            {
                if(!temp_idx)
                continue;
                cout<<" \b";
                temp_idx--;
                temp1[temp_idx]='\0';
            }
            else
            if(isalpha(ch) || isdigit(ch) || ispunct(ch))
            {
                cout<<"\b \b";
                if(temp_idx==10)
                {
                    warner("PASSWORD LENGTH EXCEEDED!!",BASE_DELAY);
                    gotoxy(10,8);
                    cout<<"ENTER NEW PASSWORD: ";
                    for(int z=1;z<=15;z++)
                    cout<<" ";
                    for(int z=1;z<=15;z++)
                    cout<<"\b";
                    if(VISIBLE)
                    cout<<temp1;
                    else
                        for(int z=0;temp1[z]!='\0';z++)
                            cout<<"*";
                    continue;
                }
                if(VISIBLE)
                cout<<ch;
                else
                cout<<"*";
                temp1[temp_idx]=ch;
                temp_idx++;
                temp1[temp_idx]='\0';
            }
            else
            if(ch==13)
            {
                prompter("                                                ");
                break;
            }
        }
        temp_idx=0;VISIBLE=false;
        
        prompter("PRESS SPACEBAR TO HIDE/SHOW THE PASSWORD");
        gotoxy(10,11);
        cout<<"CONFIRM PASSWORD: ";
        while(1)
        {
            ch=getche();
            if(ch==32)
            {
                VISIBLE=!VISIBLE;
                gotoxy(10,11);
                cout<<"CONFIRM PASSWORD: ";
                for(int z=1;z<=15;z++)
                cout<<" ";
                for(int z=1;z<=15;z++)
                cout<<"\b";
                if(VISIBLE)
                    cout<<temp2;
                else
                    for(int z=0;temp2[z]!='\0';z++)
                        cout<<"*";  
            }
            else
            if(ch==8)
            {
                if(!temp_idx)
                continue;
                cout<<" \b";
                temp_idx--;
                temp2[temp_idx]='\0';
            }
            else
            if(isalpha(ch) || isdigit(ch) || ispunct(ch))
            {
                cout<<"\b \b";
                if(temp_idx==10)
                {
                    warner("PASSWORD LENGTH EXCEEDED!!",BASE_DELAY);
                    gotoxy(10,11);
                    cout<<"CONFIRM PASSWORD: ";
                    for(int z=1;z<=15;z++)
                    cout<<" ";
                    for(int z=1;z<=15;z++)
                    cout<<"\b";
                    if(VISIBLE)
                    cout<<temp2;
                    else
                        for(int z=0;temp2[z]!='\0';z++)
                            cout<<"*";
                    continue;
                }
                if(VISIBLE)
                cout<<ch;
                else
                cout<<"*";
                temp2[temp_idx]=ch;
                temp_idx++;
                temp2[temp_idx]='\0';    
            }
            else
            if(ch==13)
            {
                prompter("                                                ");
                break;
            }
        }
        
        if(strcmp(temp1,temp2)==0)
        {
            logger("PASSWORD CHANGE SUCCESFUL!!","RETURNING TO MENU PAGE...");
            return &temp1[0];
        }
        logger("PASSWORD MISMATCH!!","PLEASE ENTER AGAIN!!");
    }
}

void bank_details_display()
{
    cls();
    bank_name_display();
    gotoxy(10,4);
    cout<<"BANK NAME:   "<<bank_details.bank_name;
    gotoxy(10,6);
    cout<<"ADDRESS:     "<<bank_details.bank_address;
    gotoxy(10,10);
    cout<<"IFSC CODE:   "<<bank_details.IFSC_code;
    gotoxy(10,12);
    cout<<"BRANCH CODE: "<<bank_details.branch_code;
    prompter("PRESS ANY KEY TO GO BACK TO HOME PAGE...");
    getc();
    prompter("                                        ");
    logger("HOME PAGE REQUEST ACCEPTED!!","REDIRECTING TO HOME PAGE...");
}

void bank_exit()
{
    cls();
    bank_name_display();
    gotoxy(14,8);
    cout<<"WHEN YOU PLAY THE GAME OF BANKS YOU EITHER LOGIN OR EXIT....";
    gotoxy(16,10);
    cout<<"...VALAR MORGHULIS!!";
    gotoxy(18,12);
    cout<<"GOODBYE!!";
    getc();
    exit(0);
}

void basic_display(System_admin admin)
{
    int c=1;
    cout<<"\t\t****WELCOME TO THE IRON BANK OF BRAAVOS****"<<endl;
    cout<<"\t\t    ------------------------------------"<<endl<<endl;
    delay(270);
    cout<<"\t\t\t    "<<c<<".SYSTEM_ADMIN"<<endl<<endl<<endl<<endl;
    c++;
    delay(270);
    cout<<"\t\t\t    "<<c<<".MANAGER"<<endl<<endl<<endl<<endl;
    delay(270);
    c++;
    cout<<"\t\t\t    "<<c<<".CUSTOMER"<<endl<<endl<<endl<<endl;
    delay(270);
    c++;
    cout<<"\t\t\t    "<<c<<".BANK DETAILS"<<endl<<endl<<endl<<endl;
    delay(270);
    c++;
    cout<<"\t\t\t    "<<c<<".EXIT";
    
}

void menu(char* M[],int num)
{
    cls();
    bank_name_display();
    cout<<endl;
    for(int i=0;i<num;i++)
    {
        delay(270);
        cout<<"\t\t  "<<i+1<<"."<<M[i]<<endl<<endl<<endl<<endl;
    }
}

struct bio_data details_input(bool FLAG)
{
    cls();
    struct bio_data ob;
    bank_name_display();
    fflush(stdin);
    
    gotoxy(10,3);
    cout<<"NAME: ";
    fflush(stdin);
    
    if(FOR_MANAGER==FLAG)
    {
        System_admin admin;
        char temp_name[30];
        strcpy(temp_name,admin.get_current_manager().get_name());
        cout<<temp_name;
        strcpy(ob.name,temp_name);
        gotoxy(7,4);
        cout<<"------------------------------------------------";
    }
    else
    gets(ob.name);
    
    START:
    fflush(stdin);
    while(1)
    {
        gotoxy(10,5);
        cout<<"ENTER AGE: ";
        cin>>ob.age;
        
        while(1)
        {
            gotoxy(10,7);
            cout<<"ENTER SEX(M/F/T): ";
            char ch;
            ch=getc();
            if(ch=='M' || ch=='m' || ch=='F' || ch=='f' || ch=='T' || ch=='t')
            {
                cout<<ch;
                ob.sex=ch;
                break;
            }
            warner("INVALID INPUT!!",BASE_DELAY);
        }  
        fflush(stdin);  
        
        while(1)
        {  
            gotoxy(10,9);
            cout<<"ENTER DATE OF BIRTH: ";
            prompter("FORMAT:dd/mm/yyyy");
            gotoxy(31,9);
            int d,m,y;
            scanf("%d%*[-/.]%d%*[-/.]%d",&d,&m,&y);
            prompter("                 ");
            time_t now=time(0);
            tm* get_date=localtime(&now);
            if(d>31 || d<=0 || (m==2 && d>29) || (m>12 || m<0) || ((1900+get_date->tm_year<y) && (1+get_date->tm_mon<m) && (get_date->tm_mday<d)))
            {
                fflush(stdin);
                warner("INVALID DATE!!",BASE_DELAY);
                gotoxy(18,9);
                cout<<"                       ";
                continue;
            }
            if((1900+get_date->tm_year-y)!=ob.age && (1900+get_date->tm_year-y)!=(ob.age-1))
            {
                gotoxy(10,9);
                cout<<"ENTER DATE OF BIRTH: ";
                cout<<"                            ";
                warner("AGE DOESN'T MATCH WITH DATE OF BIRTH!!",BASE_DELAY);
                warner("RE-ENTER ALL THE DETAILS!!",BASE_DELAY);
                goto START;
            }
            ob.dob.dd=d;
            ob.dob.mm=m;
            ob.dob.yyyy=y;
            fflush(stdin);
            break;          
        }
        gotoxy(10,11);
        cout<<"ENTER ADDRESS(single line): ";
        strcpy(ob.address,fetch());
        
        while(1)
        {
            gotoxy(10,13);
            cout<<"ENTER PHONE NO.: ";
            strcpy(ob.phone,fetch());
            if(!is_valid_phone(ob.phone))
            {
                fflush(stdin);
                warner("INAVALID PHONE NUMBER!!",BASE_DELAY);
                gotoxy(10,13);
                cout<<"                                                ";
            }
            else
            break;
        }
        fflush(stdin);
        
        gotoxy(10,15);
        cout<<"ENTER EMAIL ADDRESS: ";
        strcpy(ob.email,fetch());
        
        fflush(stdin);
        if(FOR_MANAGER==FLAG)
        logger("DETAILS ACCEPTED!!","REDIRECTING TO MENU PAGE...");
        return ob;
    }
}

void show_details(struct bio_data ob,bool value_sent)
{
    cls();
    bank_name_display();
    fflush(stdin);
    
    gotoxy(13,3);
    cout<<"NAME: ";
    fflush(stdin);
    cout<<ob.name;
    gotoxy(10,4);
    cout<<"------------------------------------------------";
    
    gotoxy(13,5);
    cout<<"AGE: ";
    cout<<ob.age;
    
    gotoxy(13,7);
    cout<<"SEX: ";
    if(ob.sex=='M' || ob.sex=='m')
    cout<<"Male";
    else
    if(ob.sex=='F' || ob.sex=='f')
    cout<<"Female";
    else
    cout<<"Transgender";

    gotoxy(13,9);
    cout<<"DATE OF BIRTH: ";
    cout<<ob.dob.dd<<"/";
    if(ob.dob.mm<10)
    cout<<"0"<<ob.dob.mm<<"/"<<ob.dob.yyyy;
    else
    cout<<ob.dob.mm<<"/"<<ob.dob.yyyy;

    gotoxy(13,11);
    cout<<"ADDRESS: ";
    cout<<ob.address;
    
    gotoxy(13,13);
    cout<<"CONTACT NO.: ";
    cout<<ob.phone;
    
    gotoxy(13,15);
    cout<<"EMAIL ADDRESS: ";
    cout<<ob.email;
    
    fflush(stdin);
    if(!value_sent)
    {
        prompter("PRESS ANY KEY TO RETURN TO MENU PAGE...");
        getc();
        prompter("                                       ");
        logger("MENU PAGE REQUEST GRANTED!!","REDIRECTING TO MENU PAGE...");   
    }

}

int choice_taker(int num,int back)
{
    int sel=0,x_coord=45;
    char ch=0;
    if(num==4)
    x_coord=57;
    else
    if(num==9)
    x_coord=60;
    else
    if(num==10)
    x_coord=40;
    else
    if(num==100)
    {
        x_coord=50;
        num=5;
    }
    gotoxy(x_coord,3);
    cout<<"<<<<<<<";
    while(1)
    {
        ch=getc(); 
        if(ch==13)
        return sel;
        else
        if(ch==27)
        return back;
        else
        if(ch==72)
        sel=(sel+num-1)%num;
        else
        if(ch==80)
        sel=(sel+1)%num;
        cout<<"\b\b\b\b\b\b\b\b        ";
        gotoxy(x_coord,4*sel+3);
        cout<<"<<<<<<<";
    }
}

void disp_login_portal(char* WINTER_IS_COMING)
{
    gotoxy(22,4);
    if(stricmp(WINTER_IS_COMING,"MANAGER")==0 || stricmp(WINTER_IS_COMING,"CUSTOMER")==0)
    cout<<"  ";
    cout<<"++++"<<WINTER_IS_COMING<<" LOGIN PORTAL++++";
    cout<<endl;
    cout<<"\t\t   -----------------------------------------";
}

bool login_page(int sel,System_admin admin,char* &id)
{   
    Manager temp;
    switch(sel)
    {
        case 0:
            return admin.try_login();
            break;
        case 1: 
            return temp.try_login();
            break;
        case 2:
            return try_customer_login(id);
            break;    
    }    
}

/*########################################################################################################*/


int main()
{
   system("mode con: cols=80 lines=44");
   System_admin admin;
   Manager manager;
   set_bank_details();
   set_admin(admin);
   //customer_approval(true);
   bool login_successful=false;
   char* user_id;
   
   while(!login_successful)
   {
       MAIN_MENU:
       cls();
       basic_display(admin);
       int choice=choice_taker(5,4);
       
       bool is_currently_available=does_file_exist(admin.get_manager_filename());
       
       if(choice==4)
       bank_exit();
       else
       if(choice==3)
       {
            bank_details_display();
            continue;
       }
       else
       if(choice==1 && !is_currently_available)
       {
            cls();
            bank_name_display();
            logger("NO MANAGERS APPOINTED!!","REDIRECTING TO HOME PAGE...");
            continue;
        }
       else
       if(choice==2)
       {
            CUSTOMER_BASE_MENU:
            char *msg[]={"SUBMIT NEW APPLICATION","LOGIN FROM EXISTING ACCOUNT","APPLICATION STATUS","HOME PAGE","EXIT"};
            menu(msg,5);
            int menu_choice=choice_taker(100,3);
            Customer new_customer_applicant;
            Manager temp_handler;
            char* id;
            Customer record;
            bool FOUND=false;
            switch(menu_choice)
            {
                case 0:
                    
                    new_customer_applicant.set_bio_data(details_input(FOR_CUSTOMER));
                    
                    while(1)
                    {
                        gotoxy(10,21);
                        fflush(stdin);
                        char c;
                        cout<<"ENTER ACCOUNT TYPE(V/C/S): ";
                        prompter("PRESS:-\n   V-->SAVINGS ACCOUNT\n   C-->CURRENT ACCOUNT\n   S-->STUDENT ACCOUNT");
                        gotoxy(37,21);
                        c=getche();
                        prompter("       \n                      \n                      \n                       ");
                        if(c!='v' && c!='V' && c!='C' && c!='v' && c!='S' && c!='s')
                        {
                            warner("INVALID INPUT!!",BASE_DELAY);
                            gotoxy(10,21);
                            cout<<"                                    ";
                        }
                        else
                        new_customer_applicant.set_account_type(c);
                        break;
                    }
                    new_customer_applicant.set_initial_status();
                    new_customer_applicant.generate_app_id();
                    new_customer_applicant.new_customer_application();
                    warner("PROCESSING DETAILS...",BASE_DELAY);
                    prompter("YOUR APPLICATION ID IS : ");
                    cout<<new_customer_applicant.get_app_id();
                    prompter("PRESS ANY KEY TO CONTINUE!!",1);
                    getc();
                    prompter("                           ",1);
                    prompter("                                      ");
                    logger("DETAILS ACCEPTED!!","REDIRECTING TO MENU PAGE...",1);
                    goto CUSTOMER_BASE_MENU;
                    break;
                case 1:
                    //LOGIN FOLLOWS AFTER THE IF BLOCK
                    break;
                case 2:
                    REFRESH:
                    cls();
                    bank_name_display();
                    gotoxy(10,10);
                    
                    int app_id;
                    cout<<"ENTER APPLICATION ID: ";
                    cin>>app_id;
                    
                    if(does_file_exist(admin.get_customer_opening_applications_filename()))
                    {
                        ifstream customer_applicants(admin.get_customer_opening_applications_filename(),ios::binary);
                        while(customer_applicants.read((char*)&record,sizeof(record)))
                        {   
                            if(record.get_app_id()==app_id)
                            {   
                                FOUND=true;
                                break;
                            }
                        }
                        customer_applicants.close();
                    }
                    
                    if(!FOUND)
                    {
                        if(does_file_exist(temp_handler.get_registered_customers_filename()))
                        {
                            ifstream registered_customers(temp_handler.get_registered_customers_filename(),ios::binary);
                            while(registered_customers.read((char*)&record,sizeof(record)))
                            {
                                if(record.get_app_id()==app_id)
                                {
                                    FOUND=true;
                                    break;
                                }
                            }
                            registered_customers.close();
                        }
                    }
                    
                    if(FOUND)
                    {
                        gotoxy(10,12);
                        cout<<"NAME: "<<record.get_name();
                        gotoxy(10,15);
                        cout<<"SYSTEM ADMIN APPROVAL STATUS: ";
                        cout<<record.get_admin_status();
                        gotoxy(10,17);
                        cout<<"MANAGER APPROVAL STATUS: ";
                        cout<<record.get_manager_status();
                        
                        if(record.get_admin_approval()==APPROVED && record.get_manager_approval()==APPROVED)
                        {
                            warner("YOUR APPLICATION HAS BEEN ACCEPTED...",BASE_DELAY,1);
                            cls();
                            bank_name_display();
                            if(record.get_is_accepted())
                                goto CUSTOMER_BASE_MENU;
                            gotoxy(10,7);
                            cout<<"NAME: "<<record.get_name();
                            gotoxy(10,9);
                            cout<<"USER ID: "<<record.get_id();
                            gotoxy(10,11);
                            char pw[12];
                            fflush(stdin);
                            cout<<"ENTER PASSWORD: ";
                            gets(pw);
                            record.set_pw(pw);
                            record.accept();
                            
                            ifstream registered_customers(temp_handler.get_registered_customers_filename(),ios::binary);
                            ofstream tempfile("TEMP_FILE.dat",ios::binary);
                            Customer ob;
                            char* user_id=record.get_id();
                            while(registered_customers.read((char*)&ob,sizeof(ob)))
                            {
                                if(!strcmp(ob.get_id(),user_id)==0)
                                tempfile.write((char*)&ob,sizeof(ob));
                                else
                                tempfile.write((char*)&record,sizeof(record));
                            }
                            tempfile.close();
                            registered_customers.close();
                            remove(temp_handler.get_registered_customers_filename());
                            rename("TEMP_FILE.dat",temp_handler.get_registered_customers_filename());
                            
                            logger("ACCOUNT CREATION SUCCESFUL!!","REDIRECTING TO PREVIOUS PAGE...",1);
                            goto CUSTOMER_BASE_MENU;    
                        }
                        else
                        {
                            prompter("PRESS ANY KEY TO RETURN TO PREVIOUS SCREEN");
                            getc();
                            prompter("                                          ");
                            warner("REDIRECTING TO PREVIOUS SCREEN...,",BASE_DELAY,1);
                            goto CUSTOMER_BASE_MENU;
                        }
                    }
                    else
                    {
                        bool FOUND_REJECTED_ID=false;
                        if(does_file_exist(REJECTED_IDS))
                        {
                            ifstream rejected_ids(REJECTED_IDS,ios::binary);
                            Customer obj;
                            while(rejected_ids.read((char*)&obj,sizeof(obj)))
                            {
                                if(obj.get_app_id()==app_id)
                                {
                                    FOUND_REJECTED_ID=true;
                                    prompter("APPLICATION REJECTED!!",1);
                                }
                            }
                            rejected_ids.close();
                        }
                        if(!FOUND_REJECTED_ID)
                        prompter("APPLICATION ID NOT FOUND!!",1);
                            
                        prompter("PRESS ESCAPE TO GO BACK TO PREVIOUS SCREEN\nPRESS ANY OTHER KEY TO ENTER AGAIN",2);
                        char ch=getc();
                        if(ch==27)
                        goto CUSTOMER_BASE_MENU;
                        else
                        goto REFRESH; 
                    } 
            
                    break;
                case 3:
                    logger("HOME PAGE REQUEST ACCEPTED!!","REDIRECTING TO HOME PAGE...");
                    goto MAIN_MENU;
                    continue;
                case 4:
                    bank_exit();
            }    
       }
       
       login_successful=login_page(choice,admin,user_id);
       
       if(login_successful)
       {    //char* msg[5];
            if(choice==0)                                                               //SYSTEM ADMIN
            {
                prompter("                                                 ");
                logger("LOGIN SUCCESSFUL!!","REDIRECTIONG TO SYSTEM ADMIN MENU PAGE...");
                is_currently_available=does_file_exist((admin.get_manager_filename()));
                SYSTEM_ADMIN_MENU_PAGE:
                if(is_currently_available)
                {
                    char* msg[]={"DELETE MANAGER ACCOUNT","PENDING ACCOUNT OPENING APPLICATIONS","LOGOUT","EXIT"};
                    menu(msg,4);
                }
                else
                {
                    char* msg[]={"CREATE MANAGER ACCOUNT","PENDING ACCOUNT OPENING APPLICATIONS","LOGOUT","EXIT"};
                    menu(msg,4);
                }
                
                if(!is_currently_available)
                {
                    prompter("SYSTEM_ADMIN and MANAGER are the two pillars of the Iron Bank Of Braavos");
                    delay(BASE_DELAY*15);
                    prompter("Without either The Bank Will Fall...                                      ");
                    delay(BASE_DELAY*4);
                    warner("Please Create A Manager!!!",BASE_DELAY);
                }
                
                int menu_choice=choice_taker(4,2);
                switch(menu_choice)                                                    //SYSTEM ADMIN MENU
                {
                    case 0:
                        
                        if(!is_currently_available)
                        {
                            cls();
                            bank_name_display();
                            gotoxy(10,5);
                            cout<<"ENTER NAME OF MANAGER TO BE APPOINTED: ";
                            manager.set_name(fetch());
                            gotoxy(10,8);
                            cout<<"ENTER MANAGER ID: ";
                            manager.set_id(fetch());
                            gotoxy(10,11);
                            cout<<"ENTER DEFAULT PASSWORD: ";
                            manager.set_pw(fetch());
                            admin.new_manager(manager);
                            logger("MANAGER ACCOUNT CREATED SUCCESSFULLY!!","REDIRECTING TO SYSTEM ADMIN MENU PAGE...");
                            is_currently_available=true;
                            cls();
                            goto SYSTEM_ADMIN_MENU_PAGE;
                        }
                        else
                        if(is_currently_available)
                        {
                            cls();
                            bank_name_display();
                            gotoxy(10,7);
                            cout<<"Press the Space bar to confirm deletion of manager account...";
                            gotoxy(10,10);
                            cout<<"Press any other key to cancel current Operation...";
                            prompter("\"Sometimes a man has to make hard choices,\n   choices that may look wrong to others...\n    but you know are right in the long run.\"\n                              -Samwell Tarly");
                            char ch=getc();
                            prompter("                                        \n                                           \n                                            \n                                            ");
                            if(ch==32)
                            {
                                admin.delete_manager();
                                logger("MANAGER ACCOUNT DELETED SUCCESFULLY!!","REDIRECTING TO SYSTEM ADMIN MENU PAGE...");
                                is_currently_available=false;
                                goto SYSTEM_ADMIN_MENU_PAGE;   
                            }
                            else
                            {
                                logger("OPERATION CANCELLED!!","REDIRECTING TO SYSTEM ADMIN MENU PAGE...");
                                goto SYSTEM_ADMIN_MENU_PAGE;
                            }
                        }
                        break;
                    case 1:
                        customer_approval(FOR_ADMIN);
                        goto SYSTEM_ADMIN_MENU_PAGE;
                        break;
                    case 2:
                        logger("LOGGING OUT!!","REDIRECTING TO HOME PAGE...");
                        login_successful=false;
                        break;
                    case 3:
                        bank_exit();
                        
                }    
            }
            else
            if(choice ==1)                                                             //MANAGER
            {
                prompter("                                                 ");
                logger("LOGIN SUCCESSFUL!!","REDIRECTIONG TO MANAGERIAL PORTAL...");
                MANAGER_MENU_PAGE:
                
                Manager current_manager=admin.get_current_manager();
                if(!current_manager.get_status_update())
                {
                   bank_name_display();
                   logger("DETAILS NOT UPDATED!!","PLEASE, UPDATE DETAILS!!");
                   current_manager=admin.get_current_manager();
                   current_manager.set_bio_data(details_input(FOR_MANAGER));
                   current_manager.set_status_update(true);
                   admin.delete_manager();
                   admin.new_manager(current_manager);
                   
                }

                char* msg[9]={"PENDING ACCOUNT OPENING APPLICATIONS","PENDING MONEY TRANSFER APPLICATIONS","ACCOUNT CLOSURE APPLICATIONS","ACCOUNT INFO","CHANGE PASSWORD","UPDATE DETAILS","RESIGN","LOGOUT","EXIT"};
                menu(msg,9);
                int menu_choice=choice_taker(9,7);
                char ch;
                
                switch(menu_choice)
                {
                    case 0:
                        customer_approval(FOR_MANAGER);
                        goto MANAGER_MENU_PAGE;
                        break;
                    case 1:
                        //transfer_approval();
                        break;
                    case 2:
                        account_closing_approval();
                        goto MANAGER_MENU_PAGE;
                        break;
                    case 3:
                        show_details(admin.get_current_manager().get_bio_data());
                        goto MANAGER_MENU_PAGE;
                    case 4:
                        char new_pw[12];
                        strcpy(new_pw,password_changer());
                        current_manager=admin.get_current_manager();
                        current_manager.set_pw(new_pw);
                        admin.delete_manager();
                        admin.new_manager(current_manager);
                        goto MANAGER_MENU_PAGE;
                        break;
                    case 5:
                        current_manager=admin.get_current_manager();
                        current_manager.set_bio_data(details_input(FOR_MANAGER));
                        current_manager.set_status_update(true);
                        admin.delete_manager();
                        admin.new_manager(current_manager);
                        break;
                    case 6:
                        cls();
                        bank_name_display();
                        gotoxy(10,7);
                        cout<<"Press the Space bar to confirm resignation...";
                        gotoxy(10,10);
                        cout<<"Press any other key to cancel current Operation...";
                        prompter("\"Sometimes a man has to make hard choices,\n   choices that may look wrong to others...\n    but you know are right in the long run.\"\n                              -Samwell Tarly");
                        ch=getc();
                        prompter("                                        \n                                           \n                                            \n                                            ");
                        if(ch==32)
                        {
                            logger("RESIGNATION ACCEPTED!!","REDIRECTING TO HOME PAGE...");
                            admin.delete_manager();
                            login_successful=false;
                        }
                        else
                        goto MANAGER_MENU_PAGE;
                    case 7:
                        logger("LOGGING OUT!!","REDIRECTING TO HOME PAGE...",2);
                        login_successful=false;
                        break;
                    case 8:
                        bank_exit();
                        
                }
                

            }
            else                                                                       //CUSTOMER
            {
                prompter("                                                 ");
                logger("LOGIN SUCCESSFUL!!","REDIRECTIONG TO CUSTOMER MENU PAGE...");
                //cout<<user_id;getc();
                
                CUSTOMER_MAIN_MENU:
                char* msg[]={"CHECK BALANCE","DEPOSIT","WITHDRAW","TRANSFER","ACCOUNT SUMMARY","ACCOUNT INFO","CHANGE PASSWORD","CLOSE ACCOUNT","LOGOUT","EXIT"};
                menu(msg,10);
                int menu_choice=choice_taker(10,8);
                Customer ob,record;
                char ch;
                
                fstream transactions;
                ofstream temp;
                ofstream closing_request_sender;
                ofstream transfers;
                transfer new_transfer;
                
                ifstream customers(manager.get_registered_customers_filename(),ios::binary);
                while(customers.read((char*)&ob,sizeof(ob)))
                {
                    if(strcmp(user_id,ob.get_id())==0)
                        break;
                }
                customers.close();
                
                long double amount;
                transaction new_transaction;
                vector<transaction> transaction_list;
                int size;
                long acc_num;
                bool acc_num_FOUND=false;
                
                switch(menu_choice)
                {
                    case 0:
                        
                        cls();
                        bank_name_display();
                        gotoxy(10,10);
                        cout<<"NAME: "<<ob.get_name();
                        gotoxy(10,14);
                        cout<<"USER ID: "<<ob.get_id();
                        gotoxy(10,18);
                        cout<<"BALANCE: "<<ob.get_balance();
                        prompter("PRESS ANY KEY TO RETURN TO PREVIOUS PAGE...",1);
                        getc();
                        prompter("                                           ",1);
                        goto CUSTOMER_MAIN_MENU;
                    
                    case 1:
                        while(1)
                        {
                            cls();
                            bank_name_display();
                            gotoxy(10,14);
                            cout<<"CURRENT BALANCE: "<<ob.get_balance();
                            gotoxy(10,17);
                            cout<<"ENTER AMOUNT TO BE DEPOSITED: ";
                            cin>>amount;
                            if(amount<0)
                            warner("INVALID AMOUNT!!",BASE_DELAY,1);
                            else
                            if(amount>100000)
                            warner("AMOUNT EXCEEDS THRESHOLD LIMIT!!",BASE_DELAY,1);
                            else 
                            break;
                        }
                        ob.set_balance(ob.get_balance()+amount);
                        
                        customers.open(manager.get_registered_customers_filename(),ios::binary);
                        temp.open("TEMP.dat",ios::app|ios::binary);
                        
                        while(customers.read((char*)&record,sizeof(record)))
                        {
                           if(strcmp(record.get_id(),user_id))
                           temp.write((char*)&record,sizeof(record));
                           else
                           temp.write((char*)&ob,sizeof(ob));
                        }
                        customers.close();
                        temp.close();
                        remove(manager.get_registered_customers_filename());
                        rename("TEMP.dat",manager.get_registered_customers_filename());
                        
                        transactions.open(manager.get_transactions_filename(),ios::app|ios::binary|ios::out);
                        new_transaction.set(user_id,amount,'+');
                        transactions.write((char*)&new_transaction,sizeof(new_transaction));
                        transactions.close();
                        
                        warner("DEPOSIT SUCCESFUL!!",BASE_DELAY);
                        gotoxy(10,20);
                        cout<<"NEW BALANCE: "<<ob.get_balance();
                        
                        prompter("PRESS ANY KEY TO RETURN TO PREVIOUS PAGE...",1);
                        getc();
                        prompter("                                           ",1);
                        goto CUSTOMER_MAIN_MENU;
                    
                    case 2:
                        while(1)
                        {
                            cls();
                            bank_name_display();
                            gotoxy(10,14);
                            cout<<"CURRENT BALANCE: "<<ob.get_balance();
                            gotoxy(10,17);
                            cout<<"ENTER AMOUNT TO BE WITHDRAWN: ";
                            cin>>amount;
                            if(amount>ob.get_balance())
                            warner("INSUFFICIENT BALANCE",BASE_DELAY,1);
                            else
                            if(amount<0)
                            warner("INVALID AMOUNT!!",BASE_DELAY,1);
                            else 
                            break;
                        }
                            
                        ob.set_balance(ob.get_balance()-amount);
                        
                        customers.open(manager.get_registered_customers_filename(),ios::binary);
                        temp.open("TEMP.dat",ios::app|ios::binary);
                        
                        while(customers.read((char*)&record,sizeof(record)))
                        {
                           if(strcmp(record.get_id(),user_id))
                           temp.write((char*)&record,sizeof(record));
                           else
                           temp.write((char*)&ob,sizeof(ob));
                        }
                        customers.close();
                        temp.close();
                        remove(manager.get_registered_customers_filename());
                        rename("TEMP.dat",manager.get_registered_customers_filename());
                        
                        transactions.open(manager.get_transactions_filename(),ios::app|ios::binary|ios::out);
                        new_transaction.set(user_id,amount,'-');
                        transactions.write((char*)&new_transaction,sizeof(new_transaction));
                        transactions.close();
                        
                        warner("WITHDRAWAL SUCCESFUL!!",BASE_DELAY,1);
                        gotoxy(10,20);
                        cout<<"NEW BALANCE: "<<ob.get_balance();
                        
                        prompter("PRESS ANY KEY TO RETURN TO PREVIOUS PAGE...",1);
                        getc();
                        prompter("                                           ",1);
                        goto CUSTOMER_MAIN_MENU;
                    
                    case 3:
                        while(1)
                        {
                            cls();
                            bank_name_display();
                            gotoxy(10,10);
                            cout<<"ENTER ACCOUNT NUMBER TO WHICH YOU WANT TO TRANSFER: ";
                            cin>>acc_num;
                            customers.open(manager.get_registered_customers_filename(),ios::binary);
                            while(customers.read((char*)&record,sizeof(record)))
                            {
                                if(record.get_account_num()==acc_num)
                                {
                                    acc_num_FOUND=true;
                                    break;
                                }
                                
                            }
                            customers.close();
                            if(acc_num_FOUND)
                            break;
                            else
                            warner("ACCOUNT NUMBER NOT FOUND!!",BASE_DELAY,1);
                        }
                        while(1)
                        {
                            gotoxy(10,13);
                            cout<<"ENTER AMOUNT: ";
                            cin>>amount;
                            if(amount<0)
                            {
                                warner("INVALID AMOUNT!!",BASE_DELAY,1);
                                gotoxy(10,13);
                                cout<<"ENTER AMOUNT:                                     ";
                            }
                            else 
                            if(amount>ob.get_balance())
                            {
                                warner("INSUFFICIENT BALANCE!!",BASE_DELAY,1);
                                gotoxy(10,13);
                                cout<<"ENTER AMOUNT:                                     ";
                            }
                            else
                            break;
                        }
                        transfers.open(manager.get_transfer_applications_filename(),ios::app|ios::binary);
                        new_transfer.set(ob.get_account_num(),acc_num,amount);
                        transfers.write((char*)&new_transfer,sizeof(new_transfer));
                        transfers.close();
                        warner("TRANSFER REQUEST PROCESSING...",BASE_DELAY,1);
                        goto CUSTOMER_MAIN_MENU;
                    
                    case 4:
                        cls();
                        bank_name_display();
                        
                        transactions.open(manager.get_transactions_filename(),ios::binary|ios::in);
                        while(transactions.read((char*)&new_transaction,sizeof(new_transaction)))
                        {
                            if(strcmp(new_transaction.get_id(),user_id)==0)
                            transaction_list.push_back(new_transaction);
                        }
                        transactions.close();
                        
                        size=transaction_list.size();
                        if(size==0)
                        {
                            logger("NO RECORDS FOUND!!","REDIRECTING TO MENU PAGE...");
                            goto CUSTOMER_MAIN_MENU;
                        }
                        
                        cout<<endl<<endl;
                        cout<<"       SL.    AMOUNT        TYPE             DATE           TIME"<<endl;
                        cout<<"      ----   --------   ------------     -------------  ------------"<<endl;
                        for(int i=0;i<size;i++)
                        {
                           cout<<"       ";
                           cout<<i+1;
                           cout<<"      ";
                           cout<<transaction_list[i].get_amount();
                           gotoxy(23,6+i*2);
                           if(transaction_list[i].get_type()=='+')
                           cout<<"   DEPOSIT         ";
                           else
                           cout<<"  WITHDRAWAL       ";
                           cout<<transaction_list[i].get_recorded_date();
                           cout<<"     "<<transaction_list[i].get_recorded_time();
                           cout<<endl<<endl;
                        }
                        cout<<endl<<endl<<endl<<endl;
                        cout<<"                                                   ";
                        cout<<"CURRENT BALANCE: "<<ob.get_balance()<<endl;
                        
                        prompter("PRESS ANY KEY TO RETURN TO MENU PAGE...",1);
                        getc();
                        goto CUSTOMER_MAIN_MENU;
                    
                    case 5:
                        show_details(ob.get_bio_data(),1);
                        gotoxy(13,17);
                        cout<<"ACCOUNT TYPE: ";
                        ch=ob.get_account_type();
                        if(ch=='S' || ch=='s')
                        cout<<"STUDENT";
                        else
                        if(ch=='C' || ch=='c')
                        cout<<"CURRENT";
                        else
                        cout<<"SAVINGS";
                        gotoxy(13,19);
                        cout<<"ACCOUNT NUMBER: "<<ob.get_account_num();
                        gotoxy(13,21);
                        cout<<"BALANCE: "<<ob.get_balance();
                        
                        prompter("PRESS ANY KEY TO RETURN TO PREVIOUS PAGE...",1);
                        getc();
                        prompter("                                           ",1);
                        goto CUSTOMER_MAIN_MENU;
                    
                    case 6:
                        ob.set_pw(password_changer());
                        customers.open(manager.get_registered_customers_filename(),ios::binary);
                        temp.open("TEMP.dat",ios::app|ios::binary);
                        
                        while(customers.read((char*)&record,sizeof(record)))
                        {
                           if(strcmp(record.get_id(),user_id))
                           temp.write((char*)&record,sizeof(record));
                           else
                           temp.write((char*)&ob,sizeof(ob));
                        }
                        customers.close();
                        temp.close();
                        remove(manager.get_registered_customers_filename());
                        rename("TEMP.dat",manager.get_registered_customers_filename());
                        goto CUSTOMER_MAIN_MENU;
                    
                    case 7:
                        cls();
                        bank_name_display();
                        
                        if(ob.get_closing_phase()==0)
                        {
                            gotoxy(10,7);
                            cout<<"Press the Space bar to submit account closing request...";
                            gotoxy(10,10);
                            cout<<"Press any other key to cancel current Operation...";
                            prompter("\"Sometimes a man has to make hard choices,\n   choices that may look wrong to others...\n    but you know are right in the long run.\"\n                              -Samwell Tarly");
                            ch=getc();
                            prompter("                                        \n                                           \n                                            \n                                            ");
                            if(ch==32)
                            {
                                ob.closing_request();
                                closing_request_sender.open(manager.get_customer_closing_applications_filename(),ios::binary|ios::app);
                                closing_request_sender.write((char*)&ob,sizeof(ob));
                                closing_request_sender.close();
                                logger("REQUEST PROCESSING!!","REDIRECTING TO MENU PAGE...");   
                            }
                            else
                            logger("REQUEST CANCELLED!!","REDIRECTING TO MENU PAGE...");
                        }
                        else
                        {
                            gotoxy(10,12);
                            if(ob.get_closing_phase()==2)
                            {
                                ob.set_closing_phase(0);
                                cout<<"PREVIOUS ACCOUNT CLOSING REQUEST DECLINED!!";
                            }
                            else
                            cout<<"PREVIOUS ACCOUNT CLOSING REQUEST PROCESSING!!";
                            prompter("PRESS ANY KEY TO GO BACK TO MAIN MENU..."); 
                            getc();            
                        }
                        
                        customers.open(manager.get_registered_customers_filename(),ios::binary);
                        temp.open("TEMP.dat",ios::app|ios::binary);
                        
                        while(customers.read((char*)&record,sizeof(record)))
                        {
                           if(strcmp(record.get_id(),user_id))
                           temp.write((char*)&record,sizeof(record));
                           else
                           temp.write((char*)&ob,sizeof(ob));
                        }
                        customers.close();
                        temp.close();
                        remove(manager.get_registered_customers_filename());
                        rename("TEMP.dat",manager.get_registered_customers_filename());
                        goto CUSTOMER_MAIN_MENU;
                    
                    case 8:
                        goto CUSTOMER_BASE_MENU;
                    
                    case 9:
                        bank_exit();
                }
                
                
            }
       }
       
       else
       {
            prompter("                                                 ");
            logger("LOGIN FAILED!!","REDIRECTING TO HOME PAGE....");
       }
   }
   
   getc();
} 

