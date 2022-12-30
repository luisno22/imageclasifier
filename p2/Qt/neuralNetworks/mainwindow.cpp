#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QString>
#include <vector>
#include <iostream>
//#include <opencv2>
#include "opennn/opennn.h"

using namespace std;
using namespace OpenNN;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    // Connect button signal to appropriate slot
    connect(ui->createDataset, SIGNAL(released()), this, SLOT(generateDataset()));
    connect(ui->trainNet, SIGNAL(released()), this, SLOT(trainNetwork()));
    connect(ui->testNet, SIGNAL(released()), this, SLOT(testNetwork()));
}

void MainWindow::generateDataset()
{
    qDebug() << "Generate Dataset";
    QString datasetDir = QString(QFileDialog::getExistingDirectory(this, "Select dataset directory"));
    datasetDir+="/";
    QString datasetDiraux = datasetDir;

    QStringList file_list = QDir(datasetDir).entryList();
    vector<int> img_vectorH; //Vector que almacena una sola imagen
    vector<int> img_vectorV; //Vector que almacena una sola imagen
    vector<vector<int>> imgsH; //Vector que almacena todas las imagenes del canal Hue
    vector<vector<int>> imgsV; //Vector que almacena todas las imagenes del canal Value
    QImage image;
    QSize size;
    QColor color;
    vector<int> clase;
    QFile fileH("../my_datasetH.csv");
    QFile fileV("../my_datasetV.csv");
    fileH.open(QFile::WriteOnly|QFile::Truncate);
    fileV.open(QFile::WriteOnly|QFile::Truncate);
    QTextStream streamH(&fileH);
    QTextStream streamV(&fileV);

    for(int i=2;i<file_list.size();i++){
        //cout<<i<<endl;
        image.load(datasetDiraux.append(file_list[i]));
        size = image.size();
        for(int y=0;y<size.height();y++){ //Recorre la imagen y la almacena en un vector
            for(int x=0;x<size.width();x++){
                color = image.pixelColor(x,y);
                img_vectorH.push_back(color.hue());
                img_vectorV.push_back(color.value());
            }
        }
        if(file_list[i].contains("dog",Qt::CaseInsensitive)){
            clase.push_back(1);
        }else if(file_list[i].contains("horse",Qt::CaseInsensitive)){
            clase.push_back(2);
        }else if(file_list[i].contains("bird",Qt::CaseInsensitive)){
            clase.push_back(3);
        }
        if(file_list[i].contains("Bird",Qt::CaseInsensitive)){
            img_vectorH.push_back(1);
            img_vectorH.push_back(0);
            img_vectorH.push_back(0);
            img_vectorV.push_back(1);
            img_vectorV.push_back(0);
            img_vectorV.push_back(0);
        }else if(file_list[i].contains("Dog",Qt::CaseInsensitive)){
            img_vectorH.push_back(0);
            img_vectorH.push_back(1);
            img_vectorH.push_back(0);
            img_vectorV.push_back(0);
            img_vectorV.push_back(1);
            img_vectorV.push_back(0);
        }else if(file_list[i].contains("Horse",Qt::CaseInsensitive)){
            img_vectorH.push_back(0);
            img_vectorH.push_back(0);
            img_vectorH.push_back(1);
            img_vectorV.push_back(0);
            img_vectorV.push_back(0);
            img_vectorV.push_back(1);
        }
        imgsH.push_back(img_vectorH);
        imgsV.push_back(img_vectorV);
        img_vectorH.clear();
        img_vectorV.clear();
        datasetDiraux=datasetDir;
    }

    /*cout<<imgsH[0].size()<<endl;
    for(uint i=0;i<imgsH[0].size();i++){
        cout<<imgsV[0][i]<<" ";
    }*/
    for(uint i=0;i<imgsH.size();i++){
        for(uint j=0;j<imgsH[i].size();j++){
            streamH<<imgsH[i][j]<<",";
            streamV<<imgsV[i][j]<<",";
        }
        streamH<<endl;
        streamV<<endl;
    }

    fileH.close();
    fileV.close();

    //Creando el primer dataset en formato xml para OpenNN, dataset de los valores Value de HSV
    DataSet data_set;
    data_set.set_data_file_name("../my_datasetV.csv");
    data_set.set_separator(DataSet::Separator::Comma);
    data_set.load_data();

    Variables* variables_pointer = data_set.get_variables_pointer();
    int classes_index = IMG_HEIGTH * IMG_WIDTH;
    variables_pointer->set_name(classes_index , "Bird");
    variables_pointer->set_use(classes_index , Variables::Target);
    variables_pointer->set_name(classes_index + 1 , "Dog");
    variables_pointer->set_use(classes_index + 1 , Variables::Target);
    variables_pointer->set_name(classes_index + 2 , "Horse");
    variables_pointer->set_use(classes_index + 2 , Variables::Target);

    cout << "Input info" << endl << variables_pointer->arrange_inputs_information() << endl;
    cout << "Target info" << endl << variables_pointer->arrange_targets_information() << endl;

    Instances* instances_pointer = data_set.get_instances_pointer();
    instances_pointer->split_random_indices(TRAINING_RATIO, SELECITON_RATIO, TESTING_RATIO);

    data_set.save("../my_datasetV.xml");

    //Creando el sgundo dataset en formato xml para OpenNN, dataset de los valores Hue de HSV
    DataSet data_set2;
    data_set2.set_data_file_name("../my_datasetH.csv");
    data_set2.set_separator(DataSet::Separator::Comma);
    data_set2.load_data();

    Variables* variables_pointer2 = data_set2.get_variables_pointer();
    int classes_index2 = IMG_HEIGTH * IMG_WIDTH;
    variables_pointer2->set_name(classes_index2 , "Bird");
    variables_pointer2->set_use(classes_index2 , Variables::Target);
    variables_pointer2->set_name(classes_index2 + 1 , "Dog");
    variables_pointer2->set_use(classes_index2 + 1 , Variables::Target);
    variables_pointer2->set_name(classes_index2 + 2 , "Horse");
    variables_pointer2->set_use(classes_index2 + 2 , Variables::Target);

    //cout << "Input info" << endl << variables_pointer2->arrange_inputs_information() << endl;
    //cout << "Target info" << endl << variables_pointer2->arrange_targets_information() << endl;

    Instances* instances_pointer2 = data_set2.get_instances_pointer();
    instances_pointer2->split_random_indices(TRAINING_RATIO, SELECITON_RATIO, TESTING_RATIO);

    data_set2.save("../my_datasetH.xml");

    qDebug() << "Datasets generated";


}

void MainWindow::trainNetwork()
{
    qDebug() << "Train Network";
    string data_set_name = "../my_datasetV.xml";
    string neural_network_name = "../neural_network.xml";
    DataSet data_set;
    data_set.load(data_set_name);
    data_set.load_data();


    //NeuralNetwork neural_network(IMG_HEIGTH*IMG_WIDTH, 10, 3);
    Vector<int> layers;
    layers.push_back(IMG_HEIGTH * IMG_WIDTH);
    layers.push_back(10);
    layers.push_back(10);
    layers.push_back(10);
    layers.push_back(3);
    MultilayerPerceptron multilayer(layers);
    multilayer.set_layer_activation_function(0, Perceptron::ActivationFunction::Logistic);
    multilayer.set_layer_activation_function(1, Perceptron::ActivationFunction::HyperbolicTangent);
    multilayer.set_layer_activation_function(2, Perceptron::ActivationFunction::Logistic);
    NeuralNetwork neural_network(multilayer);

    Variables* variables_pointer = data_set.get_variables_pointer();

    Matrix<string> inputs_information = variables_pointer->arrange_inputs_information();
    Inputs* inputs_pointer = neural_network.get_inputs_pointer();
    inputs_pointer->set_information(inputs_information);

    Matrix<string> targets_information = variables_pointer->arrange_targets_information();
    Outputs* outputs_pointer = neural_network.get_outputs_pointer();
    outputs_pointer->set_information(targets_information);

    neural_network.construct_scaling_layer();
    Vector<Statistics<double>> inputs_statistics = data_set.scale_inputs_minimum_maximum();
    ScalingLayer* scaling_layer_pointer = neural_network.get_scaling_layer_pointer();
    scaling_layer_pointer->set_statistics(inputs_statistics);
    scaling_layer_pointer->set_scaling_method(ScalingLayer::ScalingMethod::NoScaling); //Modificar para probar con los 3 tipos de escalado

    neural_network.construct_probabilistic_layer();
    ProbabilisticLayer* probabilistic_layer_pointer = neural_network.get_probabilistic_layer_pointer();
    probabilistic_layer_pointer->set_probabilistic_method(ProbabilisticLayer::ProbabilisticMethod::Softmax); //Modificar para probar los 5 tipos de probabilidad

    qDebug() << "Número de capas: " << neural_network.get_layers_number();
    qDebug() << "Neuronas en capas: " << neural_network.arrange_architecture();


    LossIndex loss_index(&neural_network, &data_set);
    loss_index.set_error_type(LossIndex::ErrorType::MEAN_SQUARED_ERROR); //Modificar para probar los 6-7 tipos de error
    TrainingStrategy training_strategy(&loss_index);
    training_strategy.set_main_type(TrainingStrategy::MainType::GRADIENT_DESCENT); //Modificar para probar los 3 algoritmos disponibles

    GradientDescent* algorithm_method_pointer = training_strategy.get_gradient_descent_pointer();
    algorithm_method_pointer->set_maximum_iterations_number(150);
    algorithm_method_pointer->set_display_period(1);
    algorithm_method_pointer->set_maximum_time(7200);

    TrainingStrategy::Results results = training_strategy.perform_training();

    neural_network.save(neural_network_name);
    qDebug() << "Entrenamiento finalizado";
}

void MainWindow::testNetwork()
{
    qDebug() << "Test Network";
    string data_set_name = "../my_datasetV.xml";
    string neural_network_name = "../neural_network.xml";
    string confusion_matrix_name = "../confusion_matrix.txt";

    DataSet data_set;
    data_set.load(data_set_name);
    data_set.load_data();

    NeuralNetwork neural_network;
    neural_network.load(neural_network_name);

    TestingAnalysis testing_analysis(&neural_network, &data_set);
    Matrix<size_t> confusion_matrix = testing_analysis.calculate_confusion();
    confusion_matrix.save(confusion_matrix_name);

    float matrix_sum = confusion_matrix.calculate_sum();
    float matrix_trace = confusion_matrix.calculate_trace();
    float accuracy_percent = (matrix_trace / matrix_sum) * 100.0;
    qDebug() << "Precisión de la red neuronal: " << accuracy_percent << " %";


}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::backpropagation(){

}*/
