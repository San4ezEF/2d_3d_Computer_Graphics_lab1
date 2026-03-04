#include <iostream>
#include <opencv2/opencv.hpp>
#include <string>
#include <ctime>

using namespace std;
using namespace cv;

// Перечисления и структуры для порядка
enum Mode { ORIGINAL = 1, GRAYSCALE = 2, SEPIA = 3, BLUR = 4 };

struct EditorState {
    Mode mode = ORIGINAL;
    int sliderVal = 50;
    string saveMsg = "";
    int msgTimer = 0;
};

// Функция обработки
void applyEffects(const Mat& input, Mat& output, EditorState& state) {
    float alpha = state.sliderVal / 100.0f;
    Mat temp;

    switch (state.mode) {
    case ORIGINAL:
        output = input.clone();
        break;
    case GRAYSCALE:
        cvtColor(input, temp, COLOR_BGR2GRAY);
        cvtColor(temp, temp, COLOR_GRAY2BGR);
        addWeighted(temp, alpha, input, 1.0 - alpha, 0, output);
        break;
    case SEPIA: {
        Mat kernel = (Mat_<float>(3, 3) <<
            0.131, 0.534, 0.272,
            0.168, 0.686, 0.349,
            0.189, 0.769, 0.393);
        transform(input, temp, kernel);
        addWeighted(temp, alpha, input, 1.0 - alpha, 0, output);
        break;
    }
    case BLUR:
        int kSize = (int)(state.sliderVal * 0.4f) * 2 + 1;
        kSize = std::max(1, kSize);
        GaussianBlur(input, output, Size(kSize, kSize), 0);
        break;
    }
}

int main() {
    setlocale(LC_ALL, "Russian");

    cout << "=== PHOTO EDITOR 2.0 ===" << endl;
    cout << "Введите путь к фото (или нажмите Enter для 'test.jpeg'): ";
    string path;
    getline(cin, path);
    if (path.empty()) path = "test.jpeg";

    Mat original = imread(path);
    if (original.empty()) {
        cerr << "Ошибка: Не удалось открыть изображение " << path << endl;
        return -1;
    }

    namedWindow("Photo Editor", WINDOW_AUTOSIZE);
    EditorState state;
    createTrackbar("Strength", "Photo Editor", &state.sliderVal, 100);

    Mat resultImg; 

    while (true) {
        applyEffects(original, resultImg, state);

        Mat displayMat = resultImg.clone();

        rectangle(displayMat, Rect(0, 0, displayMat.cols, 40), Scalar(30, 30, 30), -1);
        string info = "1:Orig 2:B&W 3:Sepia 4:Blur | S:Save ESC:Exit";
        putText(displayMat, info, Point(10, 25), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(255, 255, 255), 1);

        if (state.msgTimer > 0) {
            string saveStatus = "SAVED!";
            putText(displayMat, saveStatus, Point(displayMat.cols - 100, 25),
                FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 255, 0), 2);
            state.msgTimer--;
        }

        imshow("Photo Editor", displayMat);

        int key = waitKey(30);
        if (key == 27) break;
        if (key >= '1' && key <= '4') state.mode = (Mode)(key - '0');


        if (key == 's' || key == 'S') {
            string outName = "result_" + to_string(time(0)) + ".jpg";
            imwrite(outName, resultImg);
            state.msgTimer = 40; 
            cout << "Сохранено как: " << outName << endl;
        }
    }
    return 0;
}