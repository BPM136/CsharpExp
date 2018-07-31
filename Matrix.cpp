#include <iostream>
#include <cstdio>
#include <string>
#include <fstream>
#include <iomanip>
#include <cctype>
#include <vector>
#include <conio.h>
#include <algorithm>
#include <cstring>
#include <sstream>
#include <ctime>
#include <stack>
#include <utility>
#include <cmath>
#include <stack>
#include <cstdlib>

using namespace std;

// ====================== Class

class MatrixException : public std::exception {
public:
    string message;
    int type;
    /*
     * 1: ���Ǿ���
     * 2: �Ƿ�����
     * */

    MatrixException():message(""), type(0) {}
    MatrixException(const string msg, const int typ):message(msg), type(typ) {}

};

class Matrix {
private:
    vector<vector<double> > data;
    string name;

public:
    Matrix() {
        data = vector<vector<double> >();    //0*0�ľ���
        name = "";
    }                                       //��ʼ��
    Matrix(const string& str, const string& name);
    Matrix(const Matrix& other);

    Matrix operator+(const Matrix& right) const; // ����ӷ�
    Matrix operator-(const Matrix& right) const; // �������
    Matrix operator*(const Matrix& right) const; // ����˷�

    Matrix operator/(const double& right) const; // ���� �� ��
    Matrix operator*(const double& right) const; // ��������
    friend Matrix operator*(const double& left, const Matrix& right); // ��������(�������)

    friend istream& operator>> (istream& in, Matrix& item);
    friend ostream& operator<< (ostream& out, const Matrix& item);

    int row() const {
        return data.size();
    }
    int col() const {
        if(data.size()==0) {
            return 0;
        }
        else {
            int ans=0;
            for(int i=0; i<data.size(); i++) {
                if(data[i].size()>ans) {
                    ans=data[i].size();
                }
            }
            return ans;
        }
    }
    void setName(const string& name) {
        this->name = name;
    }
    string getName() {
        return this->name;
    }

    int rank() const; //������
    double det() const; //��������ʽ

    Matrix inv() const; //������
    Matrix adj() const; //����������
    Matrix power(int) const; //������

    bool isMatrix() const; //�����Ƿ��Ǿ���

    static unsigned short int ACCU; //��ʾ����

};
//
//void help_info(void);
//void process(string,vector<mat>&,unsigned short int);
//void divide(string,int,vector<mat>&,unsigned short int);
//unsigned short int change_accu(unsigned short int);
//vector<vector<double> > calculate(string,vector<mat>&);
//bool check(string);
//bool check_name(string);                                            //���Ⱥ��Ҳ���ʽ�Ƿ����
//vector<vector<double> > string2mat(string);                         //�ַ���ת��Ϊ����
//vector<mat>::iterator find_name(vector<mat>&,string);
//void show_mat(vector<vector<double> > matrix,unsigned short int);   //���վ�����ʾ�˾���
//int row(vector<vector<double> > matrix){return matrix.size();}      //����
//int col(vector<vector<double> > matrix){return matrix[0].size();}   //����
//vector<vector<double> > cal_adj(vector<vector<double> >);
//vector<vector<double> > cal_det(vector<vector<double> >);
//vector<vector<double> > cal_rank(vector<vector<double> >);

Matrix::Matrix(const string& str, const string& name = "") {
    vector<double> newline;
    double num;
    string numstr;
    istringstream read;
    for(int index=0, i;str[index]!='\0';index++) {
        if(isdigit(str[index])) {
            i=index;
            while(isdigit(str[i])||str[i]=='.') {i++;}
            numstr=str.substr(index,i-index);
            read.str(numstr);
            read>>num;
            if(index>=1&&str[index-1]=='-') {
                newline.push_back(-num);
            }
            else newline.push_back(num);
            index=i;
            read.clear();
        }
        if(str[index]==';'||str[index]==']') {
            this->data.push_back(newline);
            newline.clear();
        }
    }
    int col=this->data[0].size();
    for(int i=0; i<this->data.size(); i++) {
        if(col!=this->data[i].size()) {
            throw MatrixException(" === ��������ľ���Ƿ� === ", 1);
        }
    }
    this->name = name;
//    if(!isMatrix()) {
//        throw MatrixException(" === MatrixError: Not a Matrix(Wrong Input from Code) === ", 1);
//    }

} // ����, �޼��
Matrix::Matrix(const Matrix& other) {
    data = other.data;
    name = other.name;
}
istream& operator>> (istream& in, Matrix& item) {
    string buf;
    getline(in, buf);
    item.data.clear();
    try {
        item = Matrix(buf, item.name);
    } catch(MatrixException& e) {
        throw MatrixException(" === ��������ľ���Ƿ� === ", 1);
    }
    return in;
}
ostream& operator<< (ostream& out, const Matrix& item) {
    int maxnum=0,num;
    for(int i=0; i<item.row(); i++) {
        for(int j=0; j<item.col(); j++) {
            int cache=item.data[i][j];
            num=0;
            while(cache!=0){
                num++;
                cache/=10;
            }
            if(maxnum<num){
                maxnum=num;
            }
        }
    }
    // ���
    for(int i=0; i<item.row(); i++) {
        for(int j=0; j<item.col(); j++) {
            out << setiosflags(ios::fixed) << setprecision(Matrix::ACCU) << setw(Matrix::ACCU+maxnum+3) << item.data[i][j];
        }
        out << endl;
    }
    return out;
}
bool Matrix::isMatrix() const {
    int col = data[0].size();
    for(int i=0; i<data.size(); i++){
        if(data[i].size() == col){
            continue;
        }
        return false;
    }
    return true;
}
Matrix Matrix::operator+(const Matrix& right) const {
    Matrix ans;
//    if(this->errorFlag!=0 || right.errorFlag!=0){
//        ans.errorFlag = 1;
//        return ans;
//    }
    if(this->row()!=right.row() || this->col()!=right.col()) {
        throw MatrixException(" === ���󣺾���ӷ�����������������ȫ��� === ", 2);
    }
    else {
        vector<double> newline;
        for(int i=0; i<this->row(); i++) {
            for(int j=0; j<this->col(); j++) {
                newline.push_back(this->data[i][j]+right.data[i][j]);
            }
            ans.data.push_back(newline);
            newline.clear();
        }
        return ans;
    }
}
Matrix Matrix::operator-(const Matrix& right) const {
    Matrix ans;
//    if(this->errorFlag!=0 || right.errorFlag!=0){
//        ans.errorFlag = 1;
//        return ans;
//    }
    if(this->row()!=right.row() || this->col()!=right.col()) {
        throw MatrixException(" === ���󣺾����������������������ȫ��� === ", 2);
    }
    else {
        vector<double> newline;
        for(int i=0; i<this->row(); i++) {
            for(int j=0; j<this->col(); j++) {
                newline.push_back(this->data[i][j]-right.data[i][j]);
            }
            ans.data.push_back(newline);
            newline.clear();
        }
        return ans;
    }
}
Matrix Matrix::operator*(const Matrix& right) const {
    Matrix ans;
    if(this->col()!=right.row()) {
       throw MatrixException(" === ���󣺾���˷��������������������Ҳ�������� === ", 2);
    }
    vector<double> newline;
    double sum;
    for(int i=0; i<this->row(); i++) {
        for(int j=0; j<right.col(); j++) {
            sum=0;
            for(int k=0; k<this->col(); k++) {
                sum += this->data[i][k]*right.data[k][j];
            }
            newline.push_back(sum);
        }
        ans.data.push_back(newline);
        newline.clear();
    }
    return ans;
}
Matrix Matrix::operator*(const double& right) const {
    Matrix ans =* this;
    for(int i=0; i<ans.row(); i++) {
        for (int j=0; j<ans.col(); j++) {
            ans.data[i][j]*=right;
        }
    }
    return ans;
}
Matrix operator*(const double& left, const Matrix& right) {
    Matrix ans = right;
    for(int i=0; i<ans.row(); i++) {
        for (int j=0; j<ans.col(); j++) {
            ans.data[i][j]*=left;
        }
    }
    return ans;
}
Matrix Matrix::operator/(const double& right) const {
    if(fabs(right) <= 10e-8) {
        throw MatrixException(" === ���󣺾��������г�������Ϊ0 === ", 2);
    }
    Matrix ans=*this;
    for(int i=0; i<ans.row(); i++) {
        for (int j=0; j<ans.col(); j++) {
            ans.data[i][j]/=right;
        }
    }
    return ans;
}
double Matrix::det() const {
    Matrix tri = *this;
    if(this->row()!=this->col()) {
        throw MatrixException(" === ���󣺷Ƿ��󲻿�������ʽ === ", 2);
    }
    int order=this->row();
    double ans=1;
    for(int i=0; i<order; i++) {
        if(tri.data[i][i]==0) {
            int m=i+1;
            while(m<order && tri.data[m][i]==0) {
                m++;
            }
            if(m==order) {
                return 0;
            }
            else {
                double cache;
                for(int q=i; q<order; q++) {
                    cache=tri.data[m][q];
                    tri.data[m][q]=tri.data[i][q];
                    tri.data[i][q]=cache;
                } //������m�к͵�i��
            }
        }
        for(int j=i+1; j<order; j++) {
            double ratio=tri.data[j][i]/tri.data[i][i];
            for(int k=i; k<order; k++) {
                tri.data[j][k]-=ratio*tri.data[i][k];
            }
        }
    } //��tri��Ϊ�����Ǿ���
    for(int i=0; i<order; i++) {
        ans*=tri.data[i][i];
    } //����tri������ʽ
    return ans;
}
int Matrix::rank() const {
    Matrix tri = *this;
    int ans=0;
    int i=0,j=0;
    while(i<this->row() && j<this->col()) {
        if(tri.data[i][j]==0) {
            int m=i+1;
            while(m<this->row() && tri.data[m][j]==0) {
                m++;
            }
            if(m==this->row()) {
                j++;
                continue;
            }
            else {
                double cache;
                for(int q=i; q<this->col(); q++) {
                    cache=tri.data[m][q];
                    tri.data[m][q]=tri.data[i][q];
                    tri.data[i][q]=cache;
                } //������m�к͵�i��
            }
        }
        else {
            for(int p=i+1; p<this->row(); p++) {
                double ratio=tri.data[p][j]/tri.data[i][j];
                for(int q=j; q<this->col(); q++) {
                    tri.data[p][q]-=ratio*tri.data[i][q];
                }
            }
            i++;
            j++;
        } //��tri��Ϊ������
    }
    int n=0,k=0;
    while(n<this->row() && k<this->col()) {
        if(tri.data[n][k]>1e-10 || tri.data[n][k]<-1e-10) {
            ans++;
            n++;
            k++;
        }
        else {
            k++;
        }
    }
    return ans;
}
Matrix Matrix::inv() const {
    Matrix copy = *this;
    if(this->row()!=this->col()) {
        throw MatrixException(" === ���󣺷Ƿ��󲻿����� === ", 2);
    }
    Matrix ans;
    ans.data = vector<vector<double> >(row(), vector<double>(col(), 0));
    int order = this->row();
    for(int i=0; i<order; i++) {
        ans.data[i][i] = 1;
    } //������λ����ans

    for(int i=0; i<order; i++) {
        if(copy.data[i][i]==0) {
            int k=i+1;
            while(k<order || copy.data[k][i]==0) {
                k++;
            }
            if(k==order) {
                throw MatrixException(" === ���󣺷����Ⱦ��󲻿����� === ", 2);
            }
            else {
                double cache;
                for(int n=i; n<order; n++) {
                    cache=copy.data[k][n];
                    copy.data[k][n]=copy.data[i][n];
                    copy.data[i][n]=cache;
                    cache=ans.data[k][n];
                    ans.data[k][n]=ans.data[i][n];
                    ans.data[i][n]=cache;
                }
            } //������k�к͵�i��
        }
        for(int p=i+1; p<order; p++) {
            double ratio=copy.data[p][i]/copy.data[i][i];
            for(int q=0; q<order; q++) {
                copy.data[p][q]-=ratio*copy.data[i][q];
                ans.data[p][q]-=ratio*ans.data[i][q];
            }
        }
    } //copy����Ϊ������
    for(int i=1; i<order; i++) {
        for(int j=0; j<i; j++) {
            double ratio=copy.data[j][i]/copy.data[i][i];
            for(int p=0; p<order; p++) {
                copy.data[j][p]-=ratio*copy.data[i][p];
                ans.data[j][p]-=ratio*ans.data[i][p];
            }
        }
    } //copy����Ϊ�Խ�
    for(int i=0; i<order; i++) {
        for(int j=0; j<order; j++) {
            ans.data[i][j]/=copy.data[i][i];
        }
    }
    return ans;
}
Matrix Matrix::adj() const {
    Matrix ans = *this;
    if(this->row()!=this->col()) {
        throw MatrixException(" === ���󣺷Ƿ��󲻿��������� === ", 2);
    }
    return ans.inv()*ans.det();
}
Matrix Matrix::power(int exp) const {
    Matrix ans = *this, cpy = *this;
    if(this->row()!=this->col()) {
        throw MatrixException(" === ���󣺷Ƿ��󲻿����� === ", 2);
        return ans;
    }
    if(exp<0) {
        return ans.inv().power(-exp); //���-exp�η�
    }
    else if(exp==0) {
        ans.data = vector<vector<double> >(row(), vector<double>(col(), 0));
        return ans; //��λ����
    }
    else if(exp==1) {
        return ans;
    }

//    else return ans*ans.power(exp-1); //�ݹ�

//    else if(exp==2) {
//        return ans*ans;
//    }
//    else {
//        if(exp%2==0) {
//            return ans.power(exp/2).power(2);
//        }
//        else return ans.power((exp-1)/2).power(2)*ans;
//    } //������(�ݹ�)

    else {
        exp--;
        while(exp) {
            if(exp & 1) {
                ans = cpy * ans;
            }
            cpy = cpy * cpy;
            exp >>= 1;
        }
        return ans;
    } //������(�޵ݹ�)
}

struct operand{
    bool isNum; //��������������Ϊ�棬����Ǿ�����Ϊ��
    Matrix mat;
    double num;
}; //�������ṹ��

void help_info(void);
void change_accu(unsigned short int &);
void process(string);
bool check_name(string); //���Ⱥ����������Ƿ��зǷ��ַ�
bool check_exp(string); //���Ⱥ��Ҳ���ʽ�Ƿ��зǷ��ַ�
operand calc(string); //���ر��ʽ����õ��ľ���
string rev_polish(string); //���ʽת��Ϊ�沨�����ʽ
unsigned short int priority(char); //��������������ȼ�

unsigned short int Matrix::ACCU = 2;
vector<operand> opedata;
// ====================== Global Variable




// ====================== Main Function
int main() {
    // Debug
//    string n = "testName";
//    Matrix a("[1, 1.1, 1.3; 1.2, 1.2, -1; 1.1, 0, 1.3]", n);
//    cout << a;
//    try {
//        cin >> a;
//        cout << a;
//    } catch(MatrixException& e) {
//        cout << e.message << endl;
//        return 0;
//    }
//    clock_t start, end;
//    start = clock();
//    for(int i=0;i<1000;i++){
//        system("cls");
//        a.power(1000);
//    }
//    cout << a.power(2);
//    end = clock();
//    printf("Power: %ld(clock or ms), 1000 times a.power(10000000)", end - start);
    // ʹ�� .det() ��ʹ��������ʽ
//    Debug
//    string n = "testName";
//    Matrix a("[1, 1.1, 1.3; 1.2, 1.2, -1; 1.1, 0, 1.3]", n);
//    clock_t start, end;
//    start = clock();
//    for(int i=0;i<1000;i++){
//        system("cls");
//        a.power(1000);
//    }
//    cout << a.power(2);
//    end = clock();
//    printf("Power: %ld(clock or ms), 1000 times a.power(10000000)", end - start);
//     ʹ�� .det() ��ʹ��������ʽ
//    try {
//        cout << a.det() << endl;
//    } catch (int) {
//        cout << "�Ƿ��󲻿�������ʽ" << endl;
//    }

    // + - * / ^ det rank adj -(��)
    // ������(det, rank, adj)��������ɼ��㲢������������ԭ���ʽ��

    cout<<"��ӭʹ�þ����������";
    string command;
    help_info();
    cout << endl;
    while(1) {
        cout << "��������ʽ->" << endl;
        getline(cin,command);
        if(command=="quit") {
            return 0;
        }
        else if(command=="accuracy") {
            change_accu(Matrix::ACCU);
            cin.clear();
            cin.sync();
        }
        else {
            try{
                process(command);
                cin.clear();
                cin.sync();
            } catch(MatrixException& e) {
                cout << e.message <<endl;
            }
        }
    }
}

// ====================== Process Function

//  expr:
//      var = ans + [1, 2; 4, 2] * rank(ans + [5, 2; 4, 6]) / 3 + [1, 4; 4, 1] ^ 3 + inv(rank(ans))

void toRPN(string expr) {

}

void listVariable() {

}

void help_info(void) {
    cout << "ʹ�÷�����" << endl;
    cout << "1.����������������Ҫ���ո�ʽ[X1,X2,X3;X4,X5,X6]�ĸ�ʽ�������룬�ֺš�;�������У������š�[������]���������Ŀ�ʼ�ͽ�����XiΪ������" << endl;
    cout << "                   X1 X2 X3" << endl;
    cout << "����ˣ����õ����� X4 X5 X6" << endl;
    cout << "2.��ֵ�����롰a=[X1,X2,X3;X4,X5,X6]������aΪ����[X1,X2,X3;X4,X5,X6]������ÿһ����ʽ�����Ⱥ��Ҳ�ľ�����������ֵ��������������������ֻ��ȡ��Сд��ĸ��"<<endl;
    cout << "3.���㣺�Ϸ��������������(��(������),��)��(������),��+��(��),��-��(��),��*��(����˷�������),��/��(����������),��^��(��)��,��det()��(ȡ����ʽ),��rank()��(ȡ��),��adj()��(ȡ�������),��^-1��(ȡ��)��"<<endl;
    cout << "4.ʹ�þ����������� a=[2,3;1,4]" << endl;
    cout << "                   b=[2,5;1,7]" << endl;
    cout << "                   a*rank(b)" << endl;
    cout << "���õ�����:   4 6" << endl;
    cout << "              2 8" << endl;
    cout << "�ڴ�ͬʱ��a��b��ֵҲ�ᱻ���档" << endl;
    cout << "�����Ҫ��ȡ��һ�ε��������������롰ans�����������롰det(ans)�������õ���һ�ξ��������������ʽ20��" << endl;
    cout << "�����Ҫ������ʾ���ȣ������롰accuracy��" << endl;
    cout << "�����Ҫ�˳��������롰quit����" << endl;
}

void change_accu(unsigned short int& accuracy) {
    unsigned short int new_accu;
    cout << "��ǰ������" << accuracy << endl;
    cout << "���ȴ���������������С����λ��������Ϊ0����������������" << endl;
    cout << "�������µľ��ȣ����롰-1�������иı䣬��󾫶�Ϊ10��";
    cin >> new_accu;
    if(new_accu==(unsigned short int)-1) {
        return;
    }
    else if(new_accu>10) {
        cout << "���ȴ���10����������Ϊ��󾫶�10" << endl;
        accuracy = 10;
    }
    else {
        cout << "��������Ϊ" << new_accu << endl;
        accuracy = new_accu;
    }
}

bool check_name(string name) {
    for(int index=0; name[index]!='\0'; index++) {
        if(isalpha(name[index])||name[index]=='_') {
            continue;
        }
        return false;
    }
    return true;
} //���������Ƿ��зǷ��ַ�

bool check_exp(string exp) {
    for(int index=0; exp[index]!='\0'; index++) {
        if(isalnum(exp[index])||exp[index]=='['||exp[index]==']'||exp[index]=='('||exp[index]==')'||exp[index]=='+'||exp[index]=='.'
        ||exp[index]=='-'||exp[index]=='*'||exp[index]=='^'||exp[index]==','||exp[index]==';'||exp[index]=='/'||exp[index]==' '){
            continue;
        }
        return false;
    }
    return true;
} //���Ⱥ��Ҳ���ʽ�Ƿ��зǷ��ַ�

void process(string exp) {
    string command;
    operand ans;
    for(int index=0; exp[index]!='\0'; index++) {
        if(exp[index]==' ') {
            continue;
        }
        else {
            command+=exp[index];
        }
    } //���Կո�
    for(int index=0; command[index]!='\0'; index++) {
        if(command[index]=='=') {
            string name = command.substr(0, index);
            string exp = command.substr(index+1, command.size()-index);
            if(check_exp(exp)&&check_name(name)) {
                operand ans1 = calc(exp);
                vector<operand>::iterator it=opedata.begin();
                for(; it!=opedata.end(); it++) {
                    if(it->mat.getName()==name) {
                        opedata.erase(it);
                        break;
                    }
                } //����Ѿ��д����Ƶı������򸲸�
                ans1.mat.setName(name);
                opedata.push_back(ans1); //д���µı���
                cout << name << " = " << endl;
                if(ans1.isNum==1) {
                    cout << setiosflags(ios::fixed) << setprecision(Matrix::ACCU) << ans1.num << endl;
                }
                else {
                    cout<<ans1.mat;
                }
                ans=ans1;
                ans.mat.setName("ans");
                for(vector<operand>::iterator it1=opedata.begin(); it1!=opedata.end(); it1++) {
                    if(it1->mat.getName()=="ans") {
                        opedata.erase(it1);
                        break;
                    }
                }
                opedata.push_back(ans);
                return;
            } //���ڵ�ʽ�Ĵ���
            else {
                if(!check_exp(exp)) {
                    throw MatrixException(" === ���󣺵Ⱥ��Ҳ���ʽ�Ƿ� === ", 2);
                }
                else {
                    throw MatrixException(" === ���󣺵Ⱥ����������Ƿ� === ", 2);
                }
            }
        }
    }
    if(check_exp(command)==0){
        throw MatrixException(" === ��������ı��ʽ�Ƿ� === ", 2);
    }
    else {
        operand ans2=calc(command);
        if(ans2.isNum==1) {
            cout << setiosflags(ios::fixed) << setprecision(Matrix::ACCU) << ans2.num << endl;
        }
        else {
            cout<<ans2.mat;
        }
        ans=ans2;
        ans.mat.setName("ans");
        for(vector<operand>::iterator it2=opedata.begin(); it2!=opedata.end(); it2++) {
            if(it2->mat.getName()=="ans") {
                opedata.erase(it2);
                break;
            }
        }
        opedata.push_back(ans);
        return;
    }; //���ڱ��ʽ�Ĵ���
}

operand calc(string exp) {
    int index;  //�±�
    int brac_l,brac_r,brac_num,brac_index;   //����
    for(index=0;exp[index]!='\0';index++) {

        if (exp.substr(index, 5) == "rank(") {
            brac_l = index + 4;
            brac_num = 1;
            for (brac_index = index + 5; exp[brac_index] != '\0'; brac_index++) {
                if (exp[brac_index] == '(') { brac_num++; }
                else if (exp[brac_index] == ')') { brac_num--; }
                if (brac_num == 0) {
                    brac_r = brac_index;
                    break;
                }
            }
            if (exp[brac_index] == '\0') {
                throw MatrixException(" === ����rank()����δ�ҵ������� === ", 2);
                //��Ҫ������
            } else {
                stringstream replace_num;
                replace_num<<calc(exp.substr(brac_l+1,brac_r-brac_l-1)).mat.rank();
                string fin_exp=exp.substr(0,index)+replace_num.str()+exp.substr(brac_r+1,exp.size()-brac_r);
                return calc(fin_exp);
            }
        }

        if (exp.substr(index, 4) == "adj(") {
            brac_l = index + 3;
            brac_num = 1;
            for (brac_index = index + 4; exp[brac_index] != '\0'; brac_index++) {
                if (exp[brac_index] == '(') { brac_num++; }
                else if (exp[brac_index] == ')') { brac_num--; }
                if (brac_num == 0) {
                    brac_r = brac_index;
                    break;
                }
            }
            if (exp[brac_index] == '\0') {
                throw MatrixException(" === ����adj()����δ�ҵ������� === ", 2);
                //��Ҫ������
            } else {
                string new_name;
                string underline="_";
                char replace_name;
                int i=0;
                while(1) {
                    replace_name='a'+i;
                    new_name=underline+replace_name;
                    vector<operand>::iterator it=opedata.begin();
                    for(; it!=opedata.end(); it++) {
                        if(it->mat.getName() == new_name) {
                            break;
                        }
                    }
                    if(it==opedata.end()) {
                        break;
                    }
                    else {
                        i++;
                        continue;
                    }
                }
                string fin_exp=exp.substr(0,index)+new_name+exp.substr(brac_r+1,exp.size()-brac_r);
                operand new_ope;
                new_ope.isNum=0;
                new_ope.mat=calc(exp.substr(brac_l+1,brac_r-brac_l-1)).mat.adj();
                new_ope.mat.setName(new_name);
                opedata.push_back(new_ope);
                return calc(fin_exp);
            }
        }

        if (exp.substr(index, 4) == "det(") {
            brac_l = index + 3;
            brac_num = 1;
            for (brac_index = index + 4; exp[brac_index] != '\0'; brac_index++) {
                if (exp[brac_index] == '(') { brac_num++; }
                else if (exp[brac_index] == ')') { brac_num--; }
                if (brac_num == 0) {
                    brac_r = brac_index;
                    break;
                }
            }
            if (exp[brac_index] == '\0') {
                throw MatrixException(" === ����det()����δ�ҵ������� === ", 2);
                //��Ҫ������
            } else {
                stringstream replace_num;
                replace_num<<calc(exp.substr(brac_l+1,brac_r-brac_l-1)).mat.det();
                string fin_exp=exp.substr(0,index)+replace_num.str()+exp.substr(brac_r+1,exp.size()-brac_r);
                return calc(fin_exp);
            }
        }

    }
    exp = rev_polish(exp);
    stack<operand> datastk;
    istringstream read;
    int i=0;
    while(exp[i]!='\0') {
        if(exp[i]=='#'&&exp[i+1]>='0'&&exp[i+1]<='9') {
            int index=i+1;
            while(exp[index]!='\7') {
                index++;
            }
            operand data;
            data.isNum=1;
            read.str(exp.substr(i+1,index-i-1));
            read>>data.num;
            data.num=-data.num;
            datastk.push(data);
            read.clear();
            i=index+1;
            continue;
        } //�����ŵ���
        else if(exp[i]>='0'&&exp[i]<='9') {
            int index=i+1;
            while(exp[index]!='\7') {
                index++;
            }
            operand data;
            data.isNum=1;
            read.str(exp.substr(i,index-i));
            read>>data.num;
            datastk.push(data);
            read.clear();
            i=index+1;
            continue;
        } //�������ŵ���
        else if(exp[i]=='#'&&exp[i+1]=='[') {
            int index=i+1;
            while(exp[index]!='\7') {
                index++;
            }
            operand data;
            data.isNum=0;
            data.mat=Matrix(exp.substr(i+1,index-i-1),"");
            data.mat=data.mat*(-1);
            datastk.push(data);
            read.clear();
            i=index+1;
            continue;
        } //�����ŵľ���
        else if(exp[i]=='[') {
            int index=i+1;
            while(exp[index]!='\7') {
                index++;
            }
            operand data;
            data.isNum=0;
            data.mat=Matrix(exp.substr(i,index-i),"");
            datastk.push(data);
            read.clear();
            i=index+1;
            continue;
        } //�������ŵľ���
        else if(exp[i]=='#'&& (isalpha(exp[i+1])||exp[i+1]=='_') ) {
            int index=i+1;
            while(exp[index]!='\7') {
                index++;
            }
            operand data;
            vector<operand>::iterator it=opedata.begin();
            for(; it!=opedata.end(); it++) {
                if(it->mat.getName()==exp.substr(i+1,index-i-1)) {
                    data=*it;
                    datastk.push(data);
                    read.clear();
                    break;
                }
            }
            if(it==opedata.end()) {
                throw MatrixException(" === ����δ�ҵ����� === ", 2);
            }
            i=index+1;
            continue;
        } //�����ŵı���
        else if(isalpha(exp[i])||exp[i]=='_') {
            int index=i+1;
            while(exp[index]!='\7') {
                index++;
            }
            operand data;
            vector<operand>::iterator it=opedata.begin();
            for(; it!=opedata.end(); it++) {
                if(it->mat.getName()==exp.substr(i,index-i)) {
                    data=*it;
                    datastk.push(data);
                    read.clear();
                    break;
                }
            }
            if(it==opedata.end()) {
                throw MatrixException(" === ����δ�ҵ����� === ", 2);
            }
            i=index+1;
            continue;
        } //�������ŵı���
          //��ɶԲ�������ѹ��
        else if(exp[i]=='+') {
            if(datastk.empty()) {
                throw MatrixException(" === ���󣺱��ʽ�Ƿ�(�ӺŴ���) === ", 2);
            }
            operand data1=datastk.top();
            datastk.pop();
            operand data2=datastk.top();
            datastk.pop();
            operand result;
            if(data1.isNum&&data2.isNum) {
                result.isNum=1;
                result.num=data1.num+data2.num;
                datastk.push(result);
            } //�������
            else if(!data1.isNum&&!data2.isNum) {
                result.isNum=0;
                result.mat=data1.mat+data2.mat;
                datastk.push(result);
            } //�������
            else {
                throw MatrixException(" === ������������󲻿����ӷ� === ", 2);
                //���������ֺ;������
            }
            i++;
            continue;
        }
        else if(exp[i]=='-') {
            if(datastk.empty()) {
                throw MatrixException(" === ���󣺱��ʽ�Ƿ�(���Ŵ���) === ", 2);
            }
            operand data1=datastk.top();
            datastk.pop();
            operand data2=datastk.top();
            datastk.pop();
            operand result;
            if(data1.isNum&&data2.isNum) {
                result.isNum=1;
                result.num=data2.num-data1.num;
                datastk.push(result);
            } //�������
            else if(!data1.isNum&&!data2.isNum) {
                result.isNum=0;
                result.mat=data2.mat-data1.mat;
                datastk.push(result);
            } //�������
            else {
                throw MatrixException(" === ������������󲻿������� === ", 2);
                //���������ֺ;������
            }
            i++;
            continue;
        }
        else if(exp[i]=='*') {
            if(datastk.empty()) {
                throw MatrixException(" === ���󣺱��ʽ�Ƿ�(�˺Ŵ���) === ", 2);
            }
            operand data1=datastk.top();
            datastk.pop();
            operand data2=datastk.top();
            datastk.pop();
            operand result;
            if(data1.isNum&&data2.isNum) {
                result.isNum=1;
                result.num=data1.num*data2.num;
                datastk.push(result);
            } //�������
            else if(!data1.isNum&&!data2.isNum) {
                result.isNum=0;
                result.mat=data1.mat*data2.mat;
                datastk.push(result);
            } //�������
            else {
                result.isNum=0;
                if(data1.isNum) {
                    result.mat=data2.mat*data1.num;
                }
                else {
                    result.mat=data1.mat*data2.num;
                }
                datastk.push(result);
            }
            i++;
            continue;
        }
        else if(exp[i]=='/') {
            operand data1=datastk.top();
            datastk.pop();
            operand data2=datastk.top();
            datastk.pop();
            operand result;
            if(data1.isNum&&data2.isNum) {
                if(data1.num==0) {
                    throw MatrixException(" === �����㲻����Ϊ���� === ", 2);
                }
                result.isNum=1;
                result.num=data2.num/data1.num;
                datastk.push(result);
            } //�������
            else if(data1.isNum&&!data2.isNum) {
                if(data1.num==0) {
                    throw MatrixException(" === �����㲻����Ϊ���� === ", 2);
                }
                result.isNum=0;
                result.mat=data2.mat/data1.num;
                datastk.push(result);
            } //��������
            else {
                throw MatrixException(" === ���󣺾��󲻿���Ϊ���� === ", 2);
                //��������������������ֳ��Ծ���
            }
            i++;
            continue;
        }
        else if(exp[i]=='^') {
            if(datastk.empty()) {
                throw MatrixException(" === ���󣺱��ʽ�Ƿ�(�ݺŴ���) === ", 2);
            }
            operand data1=datastk.top();
            datastk.pop();
            operand data2=datastk.top();
            datastk.pop();
            operand result;
            if(data1.isNum&&!data2.isNum) {
                result.isNum=0;
                result.mat=data2.mat.power(data1.num);
                datastk.push(result);
            }
            else if(data1.isNum&&data2.isNum) {
                result.isNum=1;
                result.num=pow(data2.num,data1.num);
                datastk.push(result);
            }
            else {
                throw MatrixException(" === ���󣺾��󲻿���Ϊָ�� === ", 2);
                //�Ƿ���
            }
            i++;
            continue;
        }
    }
   return datastk.top();
}

string rev_polish(string exp) {
    string pol; //�沨�����
    stack<char> ope; //������
    for(int i=0; exp[i]!='\0'; i++) {
        if(exp[i]=='-'&&i==0||exp[i]=='-'&&(exp[i-1]=='+'||exp[i-1]=='-'||exp[i-1]=='*'||exp[i-1]=='/'||exp[i-1]=='^'||exp[i-1]=='(')) {
            exp[i]='#'; //�����Ŷ����Ǽ��Ż�Ϊ#��
        }
    }
    int i=0;
    while(exp[i]!='\0') {
        if(exp[i]==' ') {
            i++;
            continue;
        }
        else if(exp[i]=='#'&&exp[i+1]>='0'&&exp[i+1]<='9'||exp[i]>='0'&&exp[i]<='9') {
            int index=i+1;
            while(exp[index]>='0'&&exp[index]<='9'||exp[index]=='.') {
                index++;
            }
            pol+=exp.substr(i,index-i);
            pol+='\7';
            i=index;
            continue;
        } //�ҵ�����
        else if(exp[i]=='#'&&exp[i+1]=='['||exp[i]=='[') {
            int index=i+1;
            while(exp[index]!=']') {
                index++;
            }
            pol+=exp.substr(i,index-i+1);
            pol+='\7';
            i=index+1;
            continue;
        } //�ҵ�����
        else if(exp[i]=='#'&& (isalpha(exp[i+1])||exp[i+1]=='_') ||isalpha(exp[i])||exp[i]=='_') {
            int index=i+1;
            while(isalpha(exp[index])||exp[index]=='_') {
                index++;
            }
            pol+=exp.substr(i,index-i);
            pol+='\7';
            i=index;
            continue;
        } //�ҵ�����
        else if(exp[i]=='(') {
            ope.push(exp[i]);
            i++;
            continue;
        } //����������ֱ�Ӵ��������ջ
        else if(exp[i]==')') {
            while(ope.top()!='(') {
                pol+=ope.top();
                ope.pop();
            }
            ope.pop(); //����������
            i++;
            continue;
        } //����������һֱ���ֱ������������
        else if(exp[i]=='+'||exp[i]=='-'||exp[i]=='*'||exp[i]=='/'||exp[i]=='^') {
            if(ope.empty()||ope.top()=='('||priority(exp[i])>priority(ope.top())) {
                ope.push(exp[i]);
            }
            else {
                while(!ope.empty()&&ope.top()!='('&&priority(ope.top())>=priority(exp[i])) {
                    pol+=ope.top();
                    ope.pop();
                }
                ope.push(exp[i]);
            }
            i++;
            continue;
        } //���������������
        else {
            throw MatrixException(" === ����������Ƿ� === ", 2);
        }
    }
    while (!ope.empty()) {
        pol+=ope.top();
        ope.pop();
    }
    return pol;
}

unsigned short int priority(char ope) {
    if(ope=='^') {
        return 2;
    }
    else if(ope=='*'||ope=='/') {
        return 1;
    }
    else if(ope=='+'||ope=='-') {
        return 0;
    }
    else {
        throw MatrixException(" === ����������Ƿ� === ", 2);
        //��Ҫ������
    }
}

//void show_mat(vector<vector<double> > matrix,unsigned short int accu){
//    vector<vector<double> >::iterator it1;
//    vector<double>::iterator it2;
//    for(it1=matrix.begin();it1!=matrix.end();it1++){
//        for(it2=(*it1).begin();it2!=(*it1).end();it2++){
//            cout<<setiosflags(ios::fixed)<<setprecision(accu)<<setw(6)<<*it2<<" ";
//        }
//        cout<<endl;
//    }
//}

//vector<vector<double> > string2mat(string str){
//    vector<vector<double> > matrix;
//    vector<double> newline;
//    int index,i;
//    double num;
//    string numstr;
//    istringstream read;
//    for(index=0;str[index]!='\0';index++){
//        if(isdigit(str[index])){
//            i=index;
//            while(isdigit(str[i])||str[i]=='.'){i++;}
//            numstr=str.substr(index,i-index);
//            read.str(numstr);
//            read>>num;
//            newline.push_back(num);
//            index=i;
//            read.clear();
//        }
//        if(str[index]==';'||str[index]==']'){
//            matrix.push_back(newline);
//            newline.clear();
//        }
//    }
//    return matrix;
//}
//
//void process(string command,vector<mat> &matdata,unsigned short int accu){
//    char equal_sign = '=';
//    int equal_index=0;
//    for(;command[equal_index]!='\0';equal_index++){
//        if(command[equal_index]==equal_sign){
//            divide(command,equal_index,matdata,accu);
//            return;
//        }
//    }
//    if(check(command)==0){
//        cout<<"����ı��ʽ�Ƿ������������룺"<<endl;
//        return;
//    }
//    show_mat(calculate(command,matdata),accu);
//}

//void divide(string command,int equal_index,vector<mat> &matdata,unsigned short int accu){
//    for(int check_index=equal_index+1;command[check_index]!='\0';check_index++){
//        if(command[check_index]=='='){
//            cout<<"�����˹���ĵ��ںţ����������룺"<<endl;
//            return;
//        }
//    }
//    int index;
//    string divide1,divide2;
//    divide1=command.substr(0,equal_index);
//    divide2=command.substr(equal_index+1,sizeof(command)/sizeof(command[0])-equal_index);
//    if(!(check(divide2)&&check_name(divide1))){
//        cout<<"����ı��ʽ�Ƿ������������룺"<<endl;
//        return;
//    }
//    vector<mat>::iterator it;
//    for(it=matdata.begin();it!=matdata.end();it++){
//        if(it->name==divide1){
//            it->matrix=calculate(divide2,matdata);
//            cout<<divide1<<" = ";
//            show_mat(it->matrix,accu);
//            return;
//        }
//    }
//    mat newmat;
//    newmat.matrix=calculate(divide2,matdata);
//    newmat.name=divide1;
//    cout<<newmat.name<<" = ";
//    show_mat(newmat.matrix,accu);
//    matdata.push_back(newmat);
//    return;
//}
//
//vector<vector<double> > calculate(string command,vector<mat> &matdata){
//    int index;  //�±�
//    int brac_l,brac_r,brac_num,brac_index;   //����
//    for(index=0;command[index]!='\0';index++){
//
//        if(command.substr(index,5)=="rank("){
//            brac_l=index+4;
//            brac_num=1;
//            for(brac_index=index+5;command[brac_index]!='\0';brac_index++){
//                if(command[brac_index]=='('){brac_num++;}
//                else if(command[brac_index]==')'){brac_num--;}
//                if(brac_num==0){
//                    brac_r=brac_index;
//                    break;
//                }
//            }
//            if(command[brac_index]=='\0'){
//                cout<<"����ı��ʽ�Ƿ������������룺";
//                vector<vector<double> > fail;
//                return fail;
//            }
//            else{
//                string new_name;
//                stringstream ss;
//                int i=0;
//                new_name="_"+ss.str();
//                for(;find_name(matdata,new_name)!=matdata.end();i++){
//                    ss<<i;
//                    new_name="_"+ss.str();
//                }
//                string fin_name=command.substr(0,index)+new_name+command.substr(brac_r+1,sizeof(command)/sizeof(command[0])-brac_r);
//                mat temp_mat;
//                temp_mat.matrix=cal_rank(calculate(command.substr(brac_l+1,brac_r-brac_l-1),matdata));
//                temp_mat.name=new_name;
//                matdata.push_back(temp_mat);
//            }
//        }
//
//        if(command.substr(index,4)=="det("){
//            brac_l=index+3;
//            brac_num=1;
//            for(brac_index=index+4;command[brac_index]!='\0';brac_index++){
//                if(command[brac_index]=='('){brac_num++;}
//                else if(command[brac_index]==')'){brac_num--;}
//                if(brac_num==0){
//                    brac_r=brac_index;
//                    break;
//                }
//            }
//            if(command[brac_index]=='\0'){
//                cout<<"����ı��ʽ�Ƿ������������룺";
//                vector<vector<double> > fail;
//                return fail;
//            }
//            else{
//                string new_name;
//                stringstream ss;
//                int i=0;
//                new_name="_"+ss.str();
//                for(;find_name(matdata,new_name)!=matdata.end();i++){
//                    ss<<i;
//                    new_name="_"+ss.str();
//                }
//                string fin_name=command.substr(0,index)+new_name+command.substr(brac_r+1,sizeof(command)/sizeof(command[0])-brac_r);
//                mat temp_mat;
//                temp_mat.matrix=cal_det(calculate(command.substr(brac_l+1,brac_r-brac_l-1),matdata));
//                temp_mat.name=new_name;
//                matdata.push_back(temp_mat);
//            }
//        }
//
//        if(command.substr(index,4)=="adj("){
//            brac_l=index+3;
//            brac_num=1;
//            for(brac_index=index+4;command[brac_index]!='\0';brac_index++){
//                if(command[brac_index]=='('){brac_num++;}
//                else if(command[brac_index]==')'){brac_num--;}
//                if(brac_num==0){
//                    brac_r=brac_index;
//                    break;
//                }
//            }
//            if(command[brac_index]=='\0'){
//                cout<<"����ı��ʽ�Ƿ������������룺";
//                vector<vector<double> > fail;
//                return fail;
//            }
//            else{
//                string new_name;
//                stringstream ss;
//                int i=0;
//                new_name="_"+ss.str();
//                for(;find_name(matdata,new_name)!=matdata.end();i++){
//                    ss<<i;
//                    new_name="_"+ss.str();
//                }
//                string fin_name=command.substr(0,index)+new_name+command.substr(brac_r+1,sizeof(command)/sizeof(command[0])-brac_r);
//                mat temp_mat;
//                temp_mat.matrix=cal_adj(calculate(command.substr(brac_l+1,brac_r-brac_l-1),matdata));
//                temp_mat.name=new_name;
//                matdata.push_back(temp_mat);
//            }
//        }
//
//
//    }
//}
//
//vector<mat>::iterator find_name(vector<mat> &matdata,string name){
//    vector<mat>::iterator it;
//    for(it=matdata.begin();it!=matdata.end();it++){
//        if(it->name==name){return it;}
//    }
//    return it;
//}
//