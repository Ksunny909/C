#include "mainwindow.h"
#include "./ui_mainwindow.h"

//double num_first;
QString math_expression;
bool new_number = true; //отслеживаем новое число
bool waiting_for_operand = true;

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
   // double all_numbers;
   // QString new_label;

    //если новое число сбросить результат
    if (new_number || ui->result_show->text() == "0"){
      //  ui->result_show-> setText("");
        ui->result_show->setText(button->text());
        new_number = false;
    } else {
        ui->result_show->setText(ui->result_show->text() + button->text());
    }
     waiting_for_operand = false;

    //если 0.01
   /* if(ui->result_show->text().contains(".") && button->text() == "0"){
        new_label = ui->result_show->text()+ button->text();
    }
    else {
        all_numbers = (ui->result_show->text()+ button->text()).toDouble();
        new_label = QString::number(all_numbers);
    }
    ui->result_show->setText(new_label);
    //для низа
   // math_expression = new_label;
   // ui->equation_show->setText(math_expression);*/
}

void MainWindow::on_pushButton_dot_clicked()
{
    if(!(ui->result_show->text().contains('.'))){
        ui->result_show->setText( ui->result_show->text() + ".");
        //
      //  math_expression = math_expression + ".";
       // ui->equation_show->setText(math_expression);
    }
}

void MainWindow::operations()
{
    QPushButton *button = (QPushButton *)sender();

    double all_numbers;
    QString new_label;

    if(button->text() == "+/-"){
        all_numbers = (ui->result_show->text()).toDouble();
        all_numbers =  all_numbers * (-1);
        new_label = QString::number(all_numbers);
        ui->result_show->setText(new_label);

      //  ui->result_show->setText(new_label);
    }
    else     if(button->text() == "%"){
        all_numbers = (ui->result_show->text()).toDouble();
        all_numbers =  all_numbers * 0.01;
        new_label = QString::number(all_numbers);
        ui->result_show->setText(new_label);
       // ui->result_show->setText(new_label);
    }
}

void MainWindow::on_pushButton_clean_clicked()
{
    ui->pushButton_multiply->setChecked(false);
    ui->pushButton_divide->setChecked(false);
    ui->pushButton_plus->setChecked(false);
    ui->pushButton_minus->setChecked(false);

    ui->result_show->setText("0");
    //ui->result_show->setText("0");
    math_expression = "";
    ui->equation_show->setText("");
    new_number = true;
    waiting_for_operand = true;
}

double evaluateExpression(const QString& expression) {
    QStringList tokens = expression.split(' ', Qt::SkipEmptyParts);
    if (tokens.isEmpty()) return 0.0;

    double result = tokens[0].toDouble();

    for (int i = 1; i < tokens.size(); i += 2) {
        if (i + 1 >= tokens.size()) break;

        QString operation = tokens[i];
        double number = tokens[i + 1].toDouble();

        if (operation == "+") {
            result += number;
        } else if (operation == "-") {
            result -= number;
        } else if (operation == "*") {
            result *= number;
        } else if (operation == "/") {
            if (number != 0) {
                result /= number;
            } else {
                return 0; // Обработка деления на ноль
            }
        }
    }
    return result;
}

void MainWindow::on_pushButton_ecual_clicked()
{
   // double labelNumber, num_second;
    //QString new_label;

  //  QString operation = button->text();
   // num_second = ui->result_show->text().toDouble();

    //добавляем последнее число в выражение
 if(!math_expression.isEmpty() && !waiting_for_operand) {
     // Добавляем последнее число в выражение
        math_expression += " " + ui->result_show->text();

     // Вычисляем финальный результат
     double final_result = evaluateExpression(math_expression);
     ui->result_show->setText(QString::number(final_result));

        ui->equation_show->setText(math_expression);
    }

   /* if(ui->pushButton_multiply->isChecked()){
        labelNumber = num_first * num_second;
        new_label = QString::number(labelNumber);
        //new_operations = QString::number(num_first) + "*" + QString::number(num_second) ;
        ui->result_show->setText(new_label);
       // ui->result_show->setText(new_operations);
        ui->pushButton_multiply->setChecked(false);
    }
    else     if(ui->pushButton_divide->isChecked()){
        //проверка на ноль
        if (num_second == 0){
            ui->result_show->setText("null");
            //ui->result_show->setText("null");
        }
        else {
            labelNumber = num_first / num_second;
            new_label = QString::number(labelNumber);
            ui->result_show->setText(new_label);
             // ui->result_show->setText(new_label);
        }
        ui->pushButton_divide->setChecked(false);
    }
    else     if(ui->pushButton_minus->isChecked()){
        labelNumber = num_first - num_second;
        new_label = QString::number(labelNumber);
        ui->result_show->setText(new_label);
        //  ui->result_show->setText(new_label);
        ui->pushButton_minus->setChecked(false);
    }
    else     if(ui->pushButton_plus->isChecked()){
        labelNumber = num_first + num_second;
        new_label = QString::number(labelNumber);
        ui->result_show->setText(new_label);
        // ui->result_show->setText(new_label);
        ui->pushButton_plus->setChecked(false);
    }*/

    // Показываем полное выражение с результатом
   /* if(math_expression.isEmpty()) {
        ui->equation_show->setText(ui->result_show->text());
    } else {
        ui->equation_show->setText(math_expression + new_label);
    }*/

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
    //добавляем число и оператор в math_expression
   /* if(!math_expression.isEmpty()){
        math_expression = ui->result_show->text() + " " + button->text();
    } else {
        math_expression += " " + ui->result_show->text() + " " + button->text();
    }
     ui->equation_show->setText(math_expression);

    num_first = ui->result_show->text().toDouble();
    ui->result_show->setText("");
    button->setChecked(true);
    new_number = true; // Следующее число будет новым*/
    if (!waiting_for_operand) {
        // Добавляем текущее число и операцию в выражение
        if (math_expression.isEmpty()) {
            math_expression = ui->result_show->text() + " " + operation;
        } else {
            math_expression += " " + ui->result_show->text() + " " + operation;
        }

        // Вычисляем текущий результат
        double current_result = evaluateExpression(math_expression);
        ui->result_show->setText(QString::number(current_result));

        ui->equation_show->setText(math_expression);
        new_number = true;
    } else {
        // Если ждем операнд, просто меняем последнюю операцию
        if (!math_expression.isEmpty()) {
            QStringList tokens = math_expression.split(' ', Qt::SkipEmptyParts);
            if (tokens.size() >= 2) {
                tokens[tokens.size() - 1] = operation;
                math_expression = tokens.join(' ');
                ui->equation_show->setText(math_expression);
            }
        }
    }

    // Сбрасываем все кнопки операций
    ui->pushButton_plus->setChecked(false);
    ui->pushButton_minus->setChecked(false);
    ui->pushButton_multiply->setChecked(false);
    ui->pushButton_divide->setChecked(false);

    // Устанавливаем текущую кнопку как нажатую
    button->setChecked(true);
    waiting_for_operand = true;
}

