//
//  main.cpp
//  C++_MultiThreading_MySQL
//
//  Created by Mayur Jain on 2/16/17.
//  Copyright © 2017 Mayur Jain. All rights reserved.
//

#include <iostream>
#include <thread>
#include <mysql.h>
#include <string>
#include <mutex>

MYSQL *connection,mysql;
//MYSQL_RES *res;
//MYSQL_ROW row;
std::mutex mu;
int query_state;
std::string dQuotes = "\"";



void addRecords(){
   
    std::string Employee_name;
    std::string Designation;
    std::string Sex;
    std::unique_lock<std::mutex> locker(mu,std::defer_lock);
    
    locker.lock();
    std::cout<<"Adding Record into the table 'Employee' "<<std::endl;
    std::cout<<"Enter Employee name"<<std::endl;
    std::cin>>Employee_name;
    std::cout<<"Enter the Designation"<<std::endl;
    std::cin>>Designation;
    std::cout<<"Enter the Gender"<<std::endl;
    std::cin>>Sex;
    locker.unlock();
    std::string add_query = "INSERT INTO Employee (Name,Designation,Sex) VALUES ("+dQuotes+Employee_name+dQuotes+","+dQuotes+Designation+dQuotes+","+dQuotes+Sex+dQuotes+")";
    query_state = mysql_query(connection,add_query.c_str());
    if(query_state!=0)
    {
        locker.lock();
        std::cout<<mysql_error(connection)<<std::endl<<std::endl;
        locker.unlock();
    }
    
}

void updateRecords(){
    
    std::string name;
    std::string designation;
    int x;
    std::unique_lock<std::mutex> locker(mu,std::defer_lock);
    locker.lock();
    std::cout<<"Enter your Field to Update: \n -Type 1 to Update Name \n -Type 2 to Update Designation \n -Type 3 to update Sex"<<std::endl;
    std::cin>>x;
    if(x<=3 and x>=1){
    std::cout<<"Update the Records"<<std::endl;
    std::cout<<"FIELD TO BE MODIFIED"<<std::endl;
    std::cin>>name;
    std::cout<<"Where Designation or Name or Sex "<<std::endl;
    std::cin>>designation;
    }
    locker.unlock();
    
    switch (x){
    
        case 1:
                {
            std::string update_query = "UPDATE Employee SET Name = '"+name+"' Where Designation = "+dQuotes+designation+dQuotes+" OR Name ="+dQuotes+designation+dQuotes+" OR Sex="+dQuotes+designation+dQuotes;
                    query_state = mysql_query(connection,update_query.c_str());
            }
            break;
    
        case 2:
                {
            std::string update_query = "UPDATE Employee SET Designation = '"+name+"' Where Name = "+dQuotes+designation+dQuotes+" OR  Name ="+dQuotes+designation+dQuotes+" OR Sex="+dQuotes+designation+dQuotes;
                    query_state = mysql_query(connection,update_query.c_str());
                }
            break;
            
        case 3:
                {
            std::string update_query = "UPDATE Employee SET Sex = '"+name+"' Where Designation = "+dQuotes+designation+dQuotes+" OR  Name ="+dQuotes+designation+dQuotes+" OR Sex="+dQuotes+designation+dQuotes;
            query_state = mysql_query(connection,update_query.c_str());
        }
            break;
            
        default:
            locker.lock();
            std::cout<<"I Dont wish to update Any record"<<std::endl;
            locker.unlock();
    }
    
    if(query_state!=0)
    {
        locker.lock();
        std::cout<<mysql_error(connection)<<std::endl<<std::endl;
        locker.unlock();
    }
    
}

void deleteRecords(){
    
    std::string name;
    int x;
    std::unique_lock<std::mutex> Locker(mu,std::defer_lock);
    Locker.lock();
    std::cout<<"Enter the Record You Wish to Delete"<<std::endl;
    std::cout<<"Enter the Field you wish to delete the Record by: \n -Type 1 to Name \n -Type 2 to Designation \n -Type 3 to Sex"<<std::endl;
    std::cin>>x;
    if(x<=3 and x>=1){
    std::cout<<"Select the Field"<<std::endl;
    std::cin>>name;
    }
    Locker.unlock();
    
    switch (x){

        case 1:
        {
            std::string deleteQuery = "DELETE FROM Employee WHERE Name="+dQuotes+name+dQuotes;
            query_state = mysql_query(connection,deleteQuery.c_str());
        }break;
            
        case 2:
        {
            std::string deleteQuery = "DELETE FROM Employee WHERE Designation = "+dQuotes+name+dQuotes;
            
            query_state = mysql_query(connection,deleteQuery.c_str());
        }break;
            
        case 3:
        {
            std::string deleteQuery = "DELETE FROM Employee WHERE Sex = "+dQuotes+name+dQuotes;
            query_state = mysql_query(connection,deleteQuery.c_str());

        }break;
            
        default:
            Locker.lock();
            std::cout<<"I dont Wish to Delete Any Record"<<std::endl;
            Locker.unlock();
    }
}

int main(int argc, const char * argv[]) {
    
    std::string option;
    mysql_init(&mysql);
    connection = mysql_real_connect(&mysql,"localhost","mayurjain", "root","cplusplus", 0, 0, 0);
    if(connection==NULL)
    {
        std::cout<<mysql_error(&mysql)<<std::endl;
    }

    std::thread t1(addRecords);
    std::thread t2(updateRecords);
    std::thread t3(deleteRecords);
    
    t1.join();
    t2.join();
    t3.join();
    
    mysql_close(&mysql);
    return 0;
}
