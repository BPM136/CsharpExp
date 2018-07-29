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
#include <ctime>
#include <stack>
#include <utility>
#include <cmath>

using namespace std;

// ====================== Class

class MatrixException : public std::exception {
public:
    string message;
    int type;
    /*
     * 1: 不是矩阵
     * 2: 非法运算
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
        data = vector<vector<double> >();    //0*0的矩阵
        name = "";
    }                                       //初始化
    Matrix(const string& str, const string& name);
    Matrix(const Matrix& other);

    Matrix operator+(const Matrix& right) const; // 矩阵加法
    Matrix operator-(const Matrix& right) const; // 矩阵减法
    Matrix operator*(const Matrix& right) const; // 矩阵乘法

    Matrix operator/(const double& right) const; // 矩阵 除 数
    Matrix operator*(const double& right) const; // 矩阵数乘
    friend Matrix operator*(const double& left, const Matrix& right); // 矩阵数乘(数在左边)

    friend istream& operator>> (istream& in, Matrix& item);
    friend ostream& operator<< (ostream& out, const Matrix& item);

    int row() const {
        return data.size();
    }
    int col() const {
        return this->row() == 0 ? 0 : data[0].size();
    }
    void setName(const string& name) {
        this->name = name;
    }
    string getName() {
        return this->name;
    }

    int rank() const; //计算秩
    double det() const; //计算行列式

    Matrix inv() const; //计算逆
    Matrix adj() const; //计算伴随矩阵
    Matrix power(int) const; //计算幂

    bool isMatrix() const; //返回是否是矩阵

    static unsigned short int ACCU; //显示精度

};
//
//void help_info(void);
//void process(string,vector<mat>&,unsigned short int);
//void divide(string,int,vector<mat>&,unsigned short int);
//unsigned short int change_accu(unsigned short int);
//vector<vector<double> > calculate(string,vector<mat>&);
//bool check(string);
//bool check_name(string);                                            //检查等号右侧表达式是否出错
//vector<vector<double> > string2mat(string);                         //字符串转化为矩阵
//vector<mat>::iterator find_name(vector<mat>&,string);
//void show_mat(vector<vector<double> > matrix,unsigned short int);   //按照精度显示此矩阵
//int row(vector<vector<double> > matrix){return matrix.size();}      //行数
//int col(vector<vector<double> > matrix){return matrix[0].size();}   //列数
//vector<vector<double> > cal_adj(vector<vector<double> >);
//vector<vector<double> > cal_det(vector<vector<double> >);
//vector<vector<double> > cal_rank(vector<vector<double> >);

Matrix::Matrix(const string& str, const string& name = "") {
    vector<double> newline;
    double num;
    string numstr;
    istringstream read;
    for(int index=0, i;str[index]!='\0';index++){
        if(isdigit(str[index])){
            i=index;
            while(isdigit(str[i])||str[i]=='.'){i++;}
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
        if(str[index]==';'||str[index]==']'){
            this->data.push_back(newline);
            newline.clear();
        }
    }
    this->name = name;
//    if(!isMatrix()) {
//        throw MatrixException(" === MatrixError: Not a Matrix(Wrong Input from Code) === ", 1);
//    }

} // 可用, 无检查
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
        throw MatrixException(" === MatrixError: Not a Matrix(Wrong Input from Keyboard) === ", 1);
    }

    return in;
}
ostream& operator<< (ostream& out, const Matrix& item) {
//    if(item.errorFlag != 0) {
//        out << "You fucking try to output a piece of shit!" << endl;
//    }

    // 对齐
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
    // 输出
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
        throw MatrixException(" === MatrixError: Left matrix and Right matrix is not Homotypic(in operator+) === ", 2);
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
        throw MatrixException(" === MatrixError: Left matrix and Right matrix is not Homotypic(in operator-) === ", 2);
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
       throw MatrixException(" === MatrixError: The column of Left matrix is not equal to the row of Right matrix(in operator*) === ", 2);
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
        throw MatrixException(" === MatrixError: Divided by Zero(in operator/ with a number) === ", 2);
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
        throw MatrixException(" === MatrixError: Not a Square Matrix(in Function det) === ", 2);
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
                } //交换第m行和第i行
            }
        }
        for(int j=i+1; j<order; j++) {
            double ratio=tri.data[j][i]/tri.data[i][i];
            for(int k=i; k<order; k++) {
                tri.data[j][k]-=ratio*tri.data[i][k];
            }
        }
    } //将tri化为上三角矩阵
    for(int i=0; i<order; i++) {
        ans*=tri.data[i][i];
    } //计算tri的行列式
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
                } //交换第m行和第i行
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
        } //将tri化为阶梯形
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
        throw MatrixException(" === MatrixError: Not a Square Matrix(in Function det) === ", 2);
    }
    Matrix ans;
    ans.data = vector<vector<double> >(row(), vector<double>(col(), 0));
    int order = this->row();
    for(int i=0; i<order; i++) {
        ans.data[i][i] = 1;
    } //创建单位矩阵ans

    for(int i=0; i<order; i++) {
        if(copy.data[i][i]==0) {
            int k=i+1;
            while(k<order || copy.data[k][i]==0) {
                k++;
            }
            if(k==order) {
                throw MatrixException("Not a Full Rank Matrix(in Function rank)", 2);
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
            } //交换第k行和第i行
        }
        for(int p=i+1; p<order; p++) {
            double ratio=copy.data[p][i]/copy.data[i][i];
            for(int q=0; q<order; q++) {
                copy.data[p][q]-=ratio*copy.data[i][q];
                ans.data[p][q]-=ratio*ans.data[i][q];
            }
        }
    } //copy矩阵化为上三角
    for(int i=1; i<order; i++) {
        for(int j=0; j<i; j++) {
            double ratio=copy.data[j][i]/copy.data[i][i];
            for(int p=0; p<order; p++) {
                copy.data[j][p]-=ratio*copy.data[i][p];
                ans.data[j][p]-=ratio*ans.data[i][p];
            }
        }
    } //copy矩阵化为对角
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
        throw MatrixException(" === MatrixError: Not a Square Matrix(in Function adj) === ", 2);
    }
    return ans.inv()*ans.det();
}
Matrix Matrix::power(int exp) const {
    Matrix ans = *this, cpy = *this;
    if(this->row()!=this->col()) {
        throw MatrixException(" === MatrixError: Not a Square Matrix(in Function power) === ", 2);
        return ans;
    }
    if(exp<0) {
        return ans.inv().power(-exp); //逆的-exp次方
    }
    else if(exp==0) {
        ans.data = vector<vector<double> >(row(), vector<double>(col(), 0));
        return ans; //单位矩阵
    }
    else if(exp==1) {
        return ans;
    }

//    else return ans*ans.power(exp-1); //递归

//    else if(exp==2) {
//        return ans*ans;
//    }
//    else {
//        if(exp%2==0) {
//            return ans.power(exp/2).power(2);
//        }
//        else return ans.power((exp-1)/2).power(2)*ans;
//    } //快速幂(递归)

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
    } //快速幂(无递归)
}

void help_info(void);
unsigned short int change_accu(unsigned short int);
void process(string);
bool check_name(string); //检查等号左侧变量名是否有非法字符
bool check_exp(string); //检查等号右侧表达式是否有非法字符
Matrix calc(string); //返回表达式计算得到的矩阵

unsigned short int Matrix::ACCU = 2;
vector<Matrix> matdata;
// ====================== Global Variable




// ====================== Main Function
int main() {
    // Debug
    string n = "testName";
    Matrix a("[1, 1.1, 1.3; 1.2, 1.2, -1; 1.1, 0, 1.3]", n);
    cout << a;
//    try {
//        cin >> a;
//        cout << a;
//    } catch(MatrixException& e) {
//        cout << e.message << endl;
//        return 0;
//    }

    getch();

//    clock_t start, end;
//    start = clock();
//    for(int i=0;i<1000;i++){
//        system("cls");
//        a.power(1000);
//    }
//    cout << a.power(2);
//    end = clock();
//    printf("Power: %ld(clock or ms), 1000 times a.power(10000000)", end - start);
    // 使用 .det() 请使用这种形式
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
//     使用 .det() 请使用这种形式
//    try {
//        cout << a.det() << endl;
//    } catch (int) {
//        cout << "非方阵不可求行列式" << endl;
//    }

    // + - * / ^ det rank adj -(负)
    // 函数型(det, rank, adj)会优先完成计算并将其结果返回至原表达式中

    cout<<"欢迎使用矩阵计算器！";
    help_info();
    string command;
    while(1) {
        cin>>command;
        if(command=="quit") {
            return 0;
        }
        else if(command=="accuracy") {
            Matrix::ACCU=change_accu(Matrix::ACCU);
        }
        else process(command);
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
    cout << "使用方法：" << endl;
    cout << "1.输入矩阵：输入矩阵需要按照格式[X1,X2,X3;X4,X5,X6]的格式进行输入，分号“;“代表换行，中括号“[“，“]“代表矩阵的开始和结束，Xi为常数。" << endl;
    cout << "                   X1 X2 X3" << endl;
    cout << "若如此，将得到矩阵 X4 X5 X6" << endl;
    cout << "2.赋值：输入“a=[X1,X2,X3;X4,X5,X6]“即记a为矩阵[X1,X2,X3;X4,X5,X6]。对于每一个等式，将等号右侧的矩阵运算结果赋值给左侧变量。变量的名称只能取大小写字母,并且不能为rank,det,quit等功能性字符串。"<<endl;
    cout << "3.运算：合法的运算符包括“(“(左括号),“)“(右括号),“+“(加),“-“(减),“*“(乘),“^“(幂)“,“det()“(取行列式),“rank()“(取秩),“adj()“(取伴随矩阵),“^-1“(取逆),取行列式与取秩的优先级仅次于括号，并且与其右侧矩阵运算。其他一般运算符优先级照常。"<<endl;
    cout << "4.使用举例：如输入 a=[2,3;1,4]" << endl;
    cout << "                   b=[2,5;1,7]" << endl;
    cout << "                   a*rank(b)" << endl;
    cout << "将得到矩阵:   4 6" << endl;
    cout << "              2 8" << endl;
    cout << "如果想要更改显示精度，请输入accuracy" << endl;
    cout << "如果想要退出，请输入“quit“。" << endl;
}

unsigned short int change_accu(unsigned short int accuracy) {
    unsigned short int new_accu;
    cout << "当前精度是" << accuracy << endl;
    cout << "精度代表输出结果保留的小数点位数，精度为0代表输出整数结果：" << endl;
    cout << "请输入新的精度，输入“-1“不进行改变：";
    cin >> new_accu;
    if(new_accu==(unsigned short int)-1) {
        return accuracy;
    }
    else if(new_accu>10) {
        cout << "精度大于10，将精度设为最大精度10。";
        return 10;
    }
    else return new_accu;
}

bool check_name(string name) {
    for(int index=0; name[index]!='\0'; index++) {
        if(isalpha(name[index])||name[index]=='_') {
            continue;
        }
        return false;
    }
    return true;
} //检查变量名是否有非法字符

bool check_exp(string exp) {
    for(int index=0; exp[index]!='\0'; index++) {
        if(isalnum(exp[index])||exp[index]=='['||exp[index]==']'||exp[index]=='('||exp[index]==')'||exp[index]=='+'
        ||exp[index]=='-'||exp[index]=='*'||exp[index]=='^'||exp[index]==','||exp[index]==';'||exp[index]=='/'||exp[index]==' '){
            continue;
        }
        return false;
    }
    return true;
} //检查等号右侧表达式是否有非法字符

void process(string command) {
    for(int index=0; command[index]!='\0'; index++) {
        if(command[index]=='=') {
            string name = command.substr(0, index);
            string exp = command.substr(index+1, command.size()-index);
            if(check_exp(exp)&&check_name(name)) {
                Matrix ans = calc(exp);
                ans.setName(name);
                matdata.push_back(ans);
                vector<Matrix>::iterator it=matdata.begin();
                for(; it!=matdata.end(); it++) {
                    if(it->getName()=="name") {
                        matdata.erase(it);
                        break;
                    }
                }
                return;
            }
            else {
                cout << "输入的表达式非法！请重新输入：" << endl;
                return;
            }
        }
    }
    if(check_exp(command)==0){
        cout << "输入的表达式非法！请重新输入：" << endl;
        return;
    }
    else cout << calc(command);
    return;
}

Matrix calc(string exp) {

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
//        cout<<"输入的表达式非法！请重新输入："<<endl;
//        return;
//    }
//    show_mat(calculate(command,matdata),accu);
//}

//void divide(string command,int equal_index,vector<mat> &matdata,unsigned short int accu){
//    for(int check_index=equal_index+1;command[check_index]!='\0';check_index++){
//        if(command[check_index]=='='){
//            cout<<"输入了过多的等于号，请重新输入："<<endl;
//            return;
//        }
//    }
//    int index;
//    string divide1,divide2;
//    divide1=command.substr(0,equal_index);
//    divide2=command.substr(equal_index+1,sizeof(command)/sizeof(command[0])-equal_index);
//    if(!(check(divide2)&&check_name(divide1))){
//        cout<<"输入的表达式非法！请重新输入："<<endl;
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
//    int index;  //下标
//    int brac_l,brac_r,brac_num,brac_index;   //括号
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
//                cout<<"输入的表达式非法！请重新输入：";
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
//                cout<<"输入的表达式非法！请重新输入：";
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
//                cout<<"输入的表达式非法！请重新输入：";
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