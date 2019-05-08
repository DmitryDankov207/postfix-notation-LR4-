#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <iostream>
 #include <vector>
 #include <stack>
 #include <set>
#include <map>
 #include <sstream>        // istringstream

 using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    result = new QLabel(this);
    result->setText("");
    QFont font;
    font.setPixelSize(14);
    result->setFont(font);
    result->setAlignment(Qt::AlignCenter | Qt::AlignCenter);
    ui->verticalLayout->addWidget(result);
}

MainWindow::~MainWindow()
{
    delete ui;
}

 //-------------------------------------------------------------------
 double string_2_double( const string &str )
 {
   double x = 0.0;
   istringstream(str) >> x ;
   return x;
 }
 //-------------------------------------------------------------------
 // !!Проверки правильности не проводятся!!
 //-------------------------------------------------------------------
 double arithmetic_operations( const double &op1,
                               const double &op2,
                               const char   &operation )
 {
   double result = 0.0;
       switch ( operation )
       {
         case ( '+' ):  result = op1 + op2;
                        break;
         case ( '-' ):  result = op1 - op2;
                        break;
         case ( '*' ):  result = op1 * op2;
                        break;
         case ( '/' ):  result = op1 / op2;
                        break;
       }
   return result;
 }
 //-------------------------------------------------------------------
 // Массив строк expr (выражение в польской нотации) читается построчно.
 // Если считанная строка представляет число, то это число кладется в
 // стек. Если считанная строка представляет символ арифм. операции, то
 // из стека извлекаются два числа, над ними производится данная
 // операция, а её результат кладется в стек. В конце-концов в стеке
 // останется одно единственное число -- результат вычисления выражения.
 //-------------------------------------------------------------------
 double evaluate_polish_expr( const vector<string> &expr )
 {
   const set<string> operators = { "+", "-", "*", "/" };
   stack<double>     *operands   = new stack<double>();
       for ( const auto &e : expr )
       {
             if ( operators.find( e ) != operators.end() )
             {
               double  x2 = operands->top();
               operands->pop();
               double  x1 = operands->top();
               operands->pop();
               operands->push( arithmetic_operations( x1, x2, e[0] ) );
             }
             else
               operands->push( string_2_double( e ) );
       }

   return operands->top();
 }
 //-------------------------------------------------------------------

 void change_location(vector<string> & polish_expr,
                      stack<string> &operands) {
     polish_expr.push_back(operands.top());
     operands.pop();
 }

 void get_error(const stack<string> * operands, const string operation){
     if(operands->top() == operation)
         perror("invalid input!");
 }

void to_polish_expr(vector<string> & polish_expr,
                              const vector<string> &expr)
{
    const map<string, int> operators =
        { {"+", 0}, {"-", 1}, {"*", 2}, {"/", 3}, {"(", 4}, {")", 5} };
    stack<string> *operands   = new stack<string>();

    for (size_t i = 0; i < expr.size(); i++) {
            if(operators.count(expr[i]) != 0)
            {
                if(operands->size() == 0) operands->push(expr[i]);
                else {
                    if(expr[i] == ")"){
                        get_error(operands, ")");
                        while(operands->top() != "(")
                            change_location(polish_expr, *operands);
                    }
                    if(expr[i] == "(") operands->push(expr[i]);
                    else if(expr[i] == ")" && operands->top() == "(")
                        operands->pop();
                    else if(expr[i] == operands->top() || operands->top() == "*" ||
                            operands->top() == "/") {
                            change_location(polish_expr, *operands);
                            operands->push(expr[i]);
                    }
                    else
                        operands->push(expr[i]);
                }
            }
            else
                polish_expr.push_back(expr[i]);
    }
    while(operands->size() != 0){
        get_error(operands, "(");
        polish_expr.push_back(operands->top());
        operands->pop();
    }
}

double MainWindow::calculate(){
    return (string_2_double(ui->lineEdit_5->text().toUtf8().constData()) +
                ( string_2_double(ui->lineEdit->text().toUtf8().constData())
                / string_2_double(ui->lineEdit_2->text().toUtf8().constData()) -
                string_2_double(ui->lineEdit_3->text().toUtf8().constData())))
                * string_2_double(ui->lineEdit_4->text().toUtf8().constData());
}

void MainWindow::on_pushButton_clicked()
{
    vector<string> pol_expr;
    to_polish_expr(pol_expr, {"(", ui->lineEdit_5->text().toUtf8().constData(), "+",
                              "(", ui->lineEdit->text().toUtf8().constData(),
                              "/", ui->lineEdit_2->text().toUtf8().constData(), "-",
                              ui->lineEdit_3->text().toUtf8().constData(), ")", ")",
                              "*", ui->lineEdit_4->text().toUtf8().constData()});
    if(calculate() != evaluate_polish_expr(pol_expr))
        result->setText(QString("%3").arg(evaluate_polish_expr(pol_expr)) +
                        " != " + QString("%3").arg(this->calculate()));
    result->setText(QString("%3").arg(evaluate_polish_expr(pol_expr)));
}

