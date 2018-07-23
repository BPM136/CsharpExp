#include <iostream>
#include <stdio.h>
#include <string>
#include <fstream>
#include <iomanip>
#include <ctype.h>
#include <vector>
#include <conio.h>
#include <algorithm>
#include <cstring>
#include <sstream>

using namespace std;

struct mat{
    vector<vector<double> > matrix;
    string name;
};

void help_info(void);
void process(string,vector<mat>&,unsigned short int);
void divide(string,int,vector<mat>&,unsigned short int);
unsigned short int change_accu(unsigned short int);
vector<vector<double> > calculate(string,vector<mat>&);
bool check(string);
bool check_name(string);                                               //���Ⱥ��Ҳ���ʽ�Ƿ����
vector<vector<double> > string2mat(string);                         //�ַ���ת��Ϊ����
vector<mat>::iterator find_name(vector<mat>&,string);
void show_mat(vector<vector<double> > matrix,unsigned short int);   //���վ�����ʾ�˾���
int row(vector<vector<double> > matrix){return matrix.size();}      //����
int col(vector<vector<double> > matrix){return matrix[0].size();}   //����
vector<vector<double> > cal_adj(vector<vector<double> >);
vector<vector<double> > cal_det(vector<vector<double> >);
vector<vector<double> > cal_rank(vector<vector<double> >);

int main(){
    cout<<"��ӭʹ�þ����������";
    help_info();
    unsigned short int accuracy;
    vector<mat> matdata;
    string command;
    while(1){
        cin>>command;
        if(command=="quit"){
            return 0;
        }
        else if(command=="accuracy"){
            accuracy=change_accu(accuracy);
        }
        else process(command,matdata,accuracy);
    }
}

void help_info(void){
    cout<<"ʹ�÷�����"<<endl;
    cout<<"1.����������������Ҫ���ո�ʽ[X1,X2,X3;X4,X5,X6]�ĸ�ʽ�������룬�ֺš�;�������У������š�[������]���������Ŀ�ʼ�ͽ�����XiΪ������"<<endl;
    cout<<"                   X1 X2 X3"<<endl;
    cout<<"����ˣ����õ����� X4 X5 X6"<<endl;
    cout<<"2.��ֵ�����롰a=[X1,X2,X3;X4,X5,X6]������aΪ����[X1,X2,X3;X4,X5,X6]������ÿһ����ʽ�����Ⱥ��Ҳ�ľ�����������ֵ��������������������ֻ��ȡ��Сд��ĸ,���Ҳ���Ϊrank,det,quit�ȹ������ַ�����"<<endl;
    cout<<"3.���㣺�Ϸ��������������(��(������),��)��(������),��+��(��),��-��(��),��*��(��),��^��(��)��,��det()��(ȡ����ʽ),��rank()��(ȡ��),��adj()��(ȡ�������),��^-1��(ȡ��),ȡ����ʽ��ȡ�ȵ����ȼ����������ţ����������Ҳ�������㡣����һ����������ȼ��ճ���"<<endl;
    cout<<"4.ʹ�þ����������� a=[2,3;1,4]"<<endl;
    cout<<"                   b=[2,5;1,7]"<<endl;
    cout<<"                   a*rank(b)"<<endl;
    cout<<"���õ�����:   4 6"<<endl;
    cout<<"              2 8"<<endl;
    cout<<"�����Ҫ������ʾ���ȣ�������accuracy"<<endl;
    cout<<"�����Ҫ�˳��������롰quit����"<<endl;
}

unsigned short int change_accu(unsigned short int accuracy){
    unsigned short int new_accu;
    cout<<"��ǰ������"<<accuracy<<endl;
    cout<<"���ȴ���������������С����λ��������Ϊ0����������������"<<endl;
    cout<<"�������µľ��ȣ����롰-1�������иı䣺";
    cin>>new_accu;
    if(new_accu==(unsigned short int)-1){
        return accuracy;
    }
    else if(new_accu>16){
        cout<<"���ȴ���16������double���͵ľ��ȣ��Զ���������Ϊ16��";
        return 16;
    }
    else return new_accu;
}

void show_mat(vector<vector<double> > matrix,unsigned short int accu){
    vector<vector<double> >::iterator it1;
    vector<double>::iterator it2;
    for(it1=matrix.begin();it1!=matrix.end();it1++){
        for(it2=(*it1).begin();it2!=(*it1).end();it2++){
            cout<<setiosflags(ios::fixed)<<setprecision(accu)<<setw(6)<<*it2<<" ";
        }
        cout<<endl;
    }
}

vector<vector<double> > string2mat(string str){
    vector<vector<double> > matrix;
    vector<double> newline;
    int index,i;
    double num;
    string numstr;
    istringstream read;
    for(index=0;str[index]!='\0';index++){
        if(isdigit(str[index])){
            i=index;
            while(isdigit(str[i])||str[i]=='.'){i++;}
            numstr=str.substr(index,i-index);
            read.str(numstr);
            read>>num;
            newline.push_back(num);
            index=i;
            read.clear();
        }
        if(str[index]==';'||str[index]==']'){
            matrix.push_back(newline);
            newline.clear();
        }
    }
    return matrix;
}

void process(string command,vector<mat> &matdata,unsigned short int accu){
    char equal_sign = '=';
    int equal_index=0;
    for(;command[equal_index]!='\0';equal_index++){
        if(command[equal_index]==equal_sign){
            divide(command,equal_index,matdata,accu);
            return;
        }
    }
    if(check(command)==0){
        cout<<"����ı��ʽ�Ƿ������������룺"<<endl;
        return;
    }
    show_mat(calculate(command,matdata),accu);
}

void divide(string command,int equal_index,vector<mat> &matdata,unsigned short int accu){
    for(int check_index=equal_index+1;command[check_index]!='\0';check_index++){
        if(command[check_index]=='='){
            cout<<"�����˹���ĵ��ںţ����������룺"<<endl;
            return;
        }
    }
    int index;
    string divide1,divide2;
    divide1=command.substr(0,equal_index);
    divide2=command.substr(equal_index+1,sizeof(command)/sizeof(command[0])-equal_index);
    if(!(check(divide2)&&check_name(divide1))){
        cout<<"����ı��ʽ�Ƿ������������룺"<<endl;
        return;
    }
    vector<mat>::iterator it;
    for(it=matdata.begin();it!=matdata.end();it++){
        if(it->name==divide1){
            it->matrix=calculate(divide2,matdata);
            cout<<divide1<<" = ";
            show_mat(it->matrix,accu);
            return;
        }
    }
    mat newmat;
    newmat.matrix=calculate(divide2,matdata);
    newmat.name=divide1;
    cout<<newmat.name<<" = ";
    show_mat(newmat.matrix,accu);
    matdata.push_back(newmat);
    return;
}

vector<vector<double> > calculate(string command,vector<mat> &matdata){
    int index;  //�±�
    int brac_l,brac_r,brac_num,brac_index;   //����
    for(index=0;command[index]!='\0';index++){

        if(command.substr(index,5)=="rank("){
            brac_l=index+4;
            brac_num=1;
            for(brac_index=index+5;command[brac_index]!='\0';brac_index++){
                if(command[brac_index]=='('){brac_num++;}
                else if(command[brac_index]==')'){brac_num--;}
                if(brac_num==0){
                    brac_r=brac_index;
                    break;
                }
            }
            if(command[brac_index]=='\0'){
                cout<<"����ı��ʽ�Ƿ������������룺";
                vector<vector<double> > fail;
                return fail;
            }
            else{
                string new_name;
                stringstream ss;
                int i=0;
                new_name="_"+ss.str();
                for(;find_name(matdata,new_name)!=matdata.end();i++){
                    ss<<i;
                    new_name="_"+ss.str();
                }
                string fin_name=command.substr(0,index)+new_name+command.substr(brac_r+1,sizeof(command)/sizeof(command[0])-brac_r);
                mat temp_mat;
                temp_mat.matrix=cal_rank(calculate(command.substr(brac_l+1,brac_r-brac_l-1),matdata));
                temp_mat.name=new_name;
                matdata.push_back(temp_mat);
            }
        }

        if(command.substr(index,4)=="det("){
            brac_l=index+3;
            brac_num=1;
            for(brac_index=index+4;command[brac_index]!='\0';brac_index++){
                if(command[brac_index]=='('){brac_num++;}
                else if(command[brac_index]==')'){brac_num--;}
                if(brac_num==0){
                    brac_r=brac_index;
                    break;
                }
            }
            if(command[brac_index]=='\0'){
                cout<<"����ı��ʽ�Ƿ������������룺";
                vector<vector<double> > fail;
                return fail;
            }
            else{
                string new_name;
                stringstream ss;
                int i=0;
                new_name="_"+ss.str();
                for(;find_name(matdata,new_name)!=matdata.end();i++){
                    ss<<i;
                    new_name="_"+ss.str();
                }
                string fin_name=command.substr(0,index)+new_name+command.substr(brac_r+1,sizeof(command)/sizeof(command[0])-brac_r);
                mat temp_mat;
                temp_mat.matrix=cal_det(calculate(command.substr(brac_l+1,brac_r-brac_l-1),matdata));
                temp_mat.name=new_name;
                matdata.push_back(temp_mat);
            }
        }

        if(command.substr(index,4)=="adj("){
            brac_l=index+3;
            brac_num=1;
            for(brac_index=index+4;command[brac_index]!='\0';brac_index++){
                if(command[brac_index]=='('){brac_num++;}
                else if(command[brac_index]==')'){brac_num--;}
                if(brac_num==0){
                    brac_r=brac_index;
                    break;
                }
            }
            if(command[brac_index]=='\0'){
                cout<<"����ı��ʽ�Ƿ������������룺";
                vector<vector<double> > fail;
                return fail;
            }
            else{
                string new_name;
                stringstream ss;
                int i=0;
                new_name="_"+ss.str();
                for(;find_name(matdata,new_name)!=matdata.end();i++){
                    ss<<i;
                    new_name="_"+ss.str();
                }
                string fin_name=command.substr(0,index)+new_name+command.substr(brac_r+1,sizeof(command)/sizeof(command[0])-brac_r);
                mat temp_mat;
                temp_mat.matrix=cal_adj(calculate(command.substr(brac_l+1,brac_r-brac_l-1),matdata));
                temp_mat.name=new_name;
                matdata.push_back(temp_mat);
            }
        }


    }
}

vector<mat>::iterator find_name(vector<mat> &matdata,string name){
    vector<mat>::iterator it;
    for(it=matdata.begin();it!=matdata.end();it++){
        if(it->name==name){return it;}
    }
    return it;
}

bool check(string exp){
    for(int index=0;exp[index]!='\0';index++){
        if(isalnum(exp[index])||exp[index]=='['||exp[index]==']'||exp[index]=='('||exp[index]==')'||exp[index]=='+'||exp[index]=='-'||exp[index]=='*'||exp[index]=='^'||exp[index]==','||exp[index]==';'){
            continue;
        }
        return 0;
    }
    return 1;
}

bool check_name(string exp){
    for(int index=0;exp[index]!='\0';index++){
        if(isalpha(exp[index])||exp[index]=='_'){
            continue;
        }
        return 0;
    }
    return 1;
}

vector<vector<double> > cal_adj(vector<vector<double> > mat){

}

vector<vector<double> > cal_det(vector<vector<double> > mat){

}

vector<vector<double> > cal_rank(vector<vector<double> >mat){

}
