#include "mainwindow.h"
#include "./ui_mainwindow.h"

#include <QStringList>
#include <QStack>
#include <QDebug>


//float num_first;
QString math_expression; //храним варажение
bool new_number = true; //отслеживаем новое число
bool waiting_for_operand = true; //нет или есть число

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connect(ui->pushButton_0,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton_1,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton_2,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton_3,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton_4,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton_5,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton_6,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton_7,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton_8,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton_9,SIGNAL(clicked()),this,SLOT(digits_numbers()));
    connect(ui->pushButton_or,SIGNAL(clicked()),this,SLOT(operations()));
    connect(ui->pushButton_percent,SIGNAL(clicked()),this,SLOT(operations()));
    connect(ui->pushButton_clean,SIGNAL(clicked()),this,SLOT(operations()));
    connect(ui->pushButton_ecual,SIGNAL(clicked()),this,SLOT(operations()));
    connect(ui->pushButton_plus,SIGNAL(clicked()),this,SLOT(math_operations()));
    connect(ui->pushButton_minus,SIGNAL(clicked()),this,SLOT(math_operations()));
    connect(ui->pushButton_multiply,SIGNAL(clicked()),this,SLOT(math_operations()));
    connect(ui->pushButton_divide,SIGNAL(clicked()),this,SLOT(math_operations()));

    ui->pushButton_divide->setCheckable(true);
    ui->pushButton_plus->setCheckable(true);
    ui->pushButton_minus->setCheckable(true);
    ui->pushButton_multiply->setCheckable(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::digits_numbers()
{
    QPushButton *button = (QPushButton *)sender();

    //если новое число сбросить результат
    if (new_number || ui->result_show->text() == "0"){
        ui->result_show->setText(button->text());
        new_number = false;
    }
    else {
        ui->result_show->setText(ui->result_show->text() + button->text());
    }
     waiting_for_operand = false;
}

void MainWindow::on_pushButton_dot_clicked()
{
    if(!(ui->result_show->text().contains('.'))){
        ui->result_show->setText( ui->result_show->text() + ".");
    }
}

void MainWindow::operations()
{
    QPushButton *button = (QPushButton *)sender();

    float all_numbers;
    QString new_label;

    if(button->text() == "+/-"){
        all_numbers = (ui->result_show->text()).toFloat();
        all_numbers =  all_numbers * (-1);
        new_label = QString::number(all_numbers);
        ui->result_show->setText(new_label);
    }
    else if(button->text() == "%"){
        all_numbers = (ui->result_show->text()).toFloat();
        all_numbers =  all_numbers * 0.01;
        new_label = QString::number(all_numbers);
        ui->result_show->setText(new_label);
    }
}

void MainWindow::on_pushButton_clean_clicked()
{
    ui->pushButton_multiply->setChecked(false);
    ui->pushButton_divide->setChecked(false);
    ui->pushButton_plus->setChecked(false);
    ui->pushButton_minus->setChecked(false);

    ui->result_show->setText("0");
    math_expression = "";
    ui->equation_show->setText("");
    new_number = true;
    waiting_for_operand = true;
}

//создаем приоритеты в мат операциях
int getPriority(const QString& op) {
    if (op == "*" || op == "/") return 2;
    if (op == "+" || op == "-") return 1;
    return 0;
}

//выполняем операцию
float applyOperation(float a, float b, const QString& op) {
    if (op == "+") return a + b;
    if (op == "-") return a - b;
    if (op == "*") return a * b;
    if (op == "/") {
        if (b != 0) return a / b;
        else return 0; // проверка деления на ноль
    }
    return 0;
}

float evaluateExpression(const QString& expression) {
    QStringList tokens = expression.split(' ', Qt::SkipEmptyParts);

    if (tokens.isEmpty()) return 0.0;

    QStack<float> values; //стек чисел
    QStack<QString> operators; //стек операторов

    for (int i = 0; i < tokens.size(); i ++) {

        QString token = tokens[i];

        // Если токен - число
        if (token != "+" && token != "-" && token != "*" && token != "/") {
            values.push(token.toFloat());  // Кладем число в стек
        }
        // Если токен - оператор
        else {
            while (!operators.isEmpty() && getPriority(operators.top()) >= getPriority(token)) {
                // достаточно ли чисел в стеке
                if (values.size() < 2) {
                    break;
                }
                float b = values.pop();
                float a = values.pop();
                QString op = operators.pop();
                values.push(applyOperation(a, b, op));
            }
            operators.push(token);
        }
    }

    // если что-то осталось в стеке operators
    while (!operators.isEmpty()) {
        if (values.size() < 2) {
            break;
        }
        float b = values.pop();
        float a = values.pop();
        QString op = operators.pop();
        values.push(applyOperation(a, b, op));
    }
    //вывод результата
    if (values.isEmpty()) {
        return 0;
    } else {
        return values.top();
    }
}

void MainWindow::on_pushButton_ecual_clicked()
{

   if (math_expression.isEmpty()) {
       // если выражения нет, просто оставляем текущее число
       return;
   }
    if(!waiting_for_operand) {
     // если есть число для операции, то добавляем последнее число в выражение
        math_expression += " " + ui->result_show->text();

     // Вычисляем финальный результат
     float final_result = evaluateExpression(math_expression);
     ui->result_show->setText(QString::number(final_result));
    // Показываем полное выражение
        ui->equation_show->setText(math_expression);
    }

    // Сбрасываем выражение для следующей операции
    math_expression = "";
    new_number = true;
    waiting_for_operand = true;

    // Сбрасываем все кнопки операций
    ui->pushButton_plus->setChecked(false);
    ui->pushButton_minus->setChecked(false);
    ui->pushButton_multiply->setChecked(false);
    ui->pushButton_divide->setChecked(false);
}


void MainWindow::math_operations()
{
    QPushButton *button = (QPushButton *)sender();
    QString operation = button->text();

    if (!waiting_for_operand) {
        // добавляем текущее число и операцию в выражение
        if (math_expression.isEmpty()) {
            math_expression = ui->result_show->text() + " " + operation;
        } else {
            math_expression += " " + ui->result_show->text() + " " + operation;
        }

        // вычисляем результат
        float current_result = evaluateExpression(math_expression);
        ui->result_show->setText(QString::number(current_result));
        ui->equation_show->setText(math_expression);
        new_number = true;
        waiting_for_operand = true;
    }
    else {
        // если нет числа, то меняем последнюю операцию
        if (!math_expression.isEmpty()) {
            QStringList tokens = math_expression.split(' ', Qt::SkipEmptyParts);
            if (tokens.size() >= 2) { //есть число и оператор
                tokens[tokens.size() - 1] = operation; //последний элемент массива равен новой операции
                math_expression = tokens.join(' ');
                ui->equation_show->setText(math_expression);
            }
        }
    }

    // сбрасываем все кнопки операций
    ui->pushButton_plus->setChecked(false);
    ui->pushButton_minus->setChecked(false);
    ui->pushButton_multiply->setChecked(false);
    ui->pushButton_divide->setChecked(false);

    // Уустанавливаем текущую кнопку как нажатую
    button->setChecked(true);
}

