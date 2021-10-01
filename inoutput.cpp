/*****************************************************************//**
 * \file  inoutput.cpp
 * \brief  Файл реализации функций ввода/вывода команд и изображений.
 * \author Преучиль Н.А.
 * \date   12 January 2021
 *********************************************************************/
#include "inoutput.h"

/**
 * \brief Обработка файла "command.txt".
 * \details Функция считывает содержимое файла "command.txt", создаёт и записывает в память указатель на очередь команд,
 * а так же количество введённых команд и изображений.
 * \details Требования и органичения:
 * - Количество команд не превышает 100;
 * \details Этапы работы функции:
 * - инициализация локальных переменных.
 * - выделение памяти для очереди команд.
 * - создание ссылки на файл с командами.
 * - считывание количества картинок и команд.
 * - считывание команд.
 * - удаление ссылки на файл.
 * \param InputResult* out - указатель на структуру с результатами вывода, в которую и будут сохраняться результаты
 * работы данной подпрограммы.
 * \return void.
 */
void InputCommands(InputResult* out) {
	// Инициализация локальных переменных.
	FILE* ptr_CommInputFile;																	
	int CommCount;											// Количество введённых команд.
	int PicsCount;											// Количество введённых изображений.
	char WrongFormatPics[310];								// Строка для списка изображений, к которым будут применяться команды.
	// Выделение памяти для очереди команд.
	QueueElement* ptr_QueueArr;
	ptr_QueueArr = new QueueElement[100];
	out->ptr_QueueArr = ptr_QueueArr;						// Ссылка на очередь команд передаётся в выходную структуру out.
	// Создание ссылки на файл с командами.
	ptr_CommInputFile = fopen("command.txt", "rt");
	// Считывание количества картинок и команд.
	(void)fscanf(ptr_CommInputFile, "%d %d\n", &PicsCount, &CommCount);
	out->PicsCount = PicsCount;								// Количество изображений передаётся в выходную структуру out.
	out->CommCount = CommCount;								// Количество команд передаётся в выходную структуру out.
	// Считывание команд.
	for (int i = 0; i < CommCount; i++) {
		(void)fscanf(ptr_CommInputFile, "%s : %s : ", &WrongFormatPics, &ptr_QueueArr[i].CommName);
		fgets(ptr_QueueArr[i].CommArgue, 5, ptr_CommInputFile);
		// После каждого считывания команды, вызывается функция TransformNums, преобразующая строку изображений, к которым будут применяться команды (подробнее в описании к функции TransformNums).
		out->ptr_QueueArr[i].CommPicsCount = TransformNums(&WrongFormatPics[0], ptr_QueueArr[i].CommPics);
	}
	// Удаление ссылки на файл.
	fclose(ptr_CommInputFile);
	return;
}

/**
 * \brief Парсер строки.
 * \details Функция преобразует строку вида "1,2,3,4" из файла "command.txt" в массив чисел.
 * \details Указатель передвигается по строке и пока не доходит до запятой, переносит числа в промежуточную строку. 
 * Затем функция считывает числовое значение из строки. Далее алогритм повторяется.
 * \details Этапы работы функции:
 * - инициализация локальных переменных.
 * - включение цикла, чтобы указатель передвигался, пока не встретит символ конца строки. 
 * - 
 * \param char* ptr_WrongStr - указатель на строку неправильного формата.
 * \param char* ptr_ResultArr - указатель на массив чисел для заполнения.
 * \return Функция возвращает количество элементов, записанных в массив чисел.
 */
int TransformNums(char* ptr_WrongStr, int* ptr_ResultArr) {
	// Инициализация локальных переменных.
	char* ptr_Temp;
	ptr_Temp = &ptr_WrongStr[0];						// Указатель на входную строку.
	char CurrentStr[5];									// Промежуточная строка.
	int CurCounter = 0;									// Счётчик промежуточной строки.
	int ResCounter = 0;									// Счётчик чисел в выходном массиве.
	// Запуск цикла.
	while (*ptr_Temp != '\0') {
		// Условие проверяющее, не дошёл ли указатель до запятой.
		if (*ptr_Temp != ',') {
			// Перенос чисел в промежуточную строку.
			CurrentStr[CurCounter] = *ptr_Temp;
			CurCounter++;
		}
		else {
			// Перенос числового значения строки в выходной массив чисел.
			ptr_ResultArr[ResCounter] = atoi(CurrentStr);
			ResCounter++;
			// Очистка промежуточной строки.
			memset(CurrentStr, 0, 4);
			// Обнуление счётчика промежуточной строки.
			CurCounter = 0;
		}
		ptr_Temp++;
	}
	// Обработка оставшейся строки, которая осталась, потому что строка заканчивается числом, а не запятой.
	ptr_ResultArr[ResCounter] = atoi(CurrentStr);
	return ResCounter + 1;
}

/**
 * \brief Генератор имён файлов.
 * \details Функция генерирует имена файлов для ввода/вывода и открывает файл.
 * \details Функция имеет 2 режима работы:
 * - 'i' - создание имени формата "inputxx.bmp";
 * - 'o' - создание имени формата "outputxx.bmp";
 * \details Этапы работы функции:
 * - создание строки для имени файла.
 * - заполнение строки.
 * - открытие ссылки.
 * \param int Num - Номер файла.
 * \param char flag - символ определяющий режим работы функции.
 * \return Функция возвращает ссылку на файл;
 */
FILE* FileNameGenerator(int Num, char flag) {
	FILE* ptr_PicInputFile;
	// Создание строки для имени файла.
	char FileName[15];
	if (flag == 'i') {
		// Заполнение строки.
		sprintf(FileName, "input%d%d.bmp", Num / 10, Num % 10);
		// Открытие ссылки на файл входного изображения.
		ptr_PicInputFile = fopen(FileName, "rb");
	}
	else if (flag == 'o') {
		// Заполнение строки.
		sprintf(FileName, "output%d%d.bmp", Num / 10, Num % 10);
		// Открытие ссылки на файл выходного изображения.
		ptr_PicInputFile = fopen(FileName, "wb");
	}
	else
		ptr_PicInputFile = NULL;
#ifdef DEBUG
	cout << FileName << endl;							// Контроль сгенерированных имён файлов при отладке.
#endif DEBUG

	return ptr_PicInputFile;
}

/**
 * \brief Ввод изображений.
 * \details Функция побитово считывает изображения изображения из файлов "inputxx.bmp" и 
 * записывает в структуру с результатами ссылку на массив с изображениями.
 * \details Требования и органичения:
 * - Разрешение изображений не превышает 1000 на 1000 пикселей;
 * - Количество файлов не более 100
 * \details Этапы работы функции:
 * - инициализация локальных переменных.
 * - выделение памяти для массива изображений.
 * - запуск цикла для считывания всех изображений.
 * - побиовое считывание каждого изображения.
 * \param InputResult* out - указатель на структуру с результатами вывода, 
 *  в которую и будет сохраняться ссылка на изображения.
 * \return void.
 */
void InputPictures(InputResult* out) {
	// Инициализация локальных переменных.
	FILE* file;
	int PicCount = out->PicsCount;									// Программа берёт количество изображений из переданной ей выходной структуры out.
	// Выделение памяти для массива изображений.
	Pic* ptr_Pics;
	ptr_Pics = new Pic[100];
	out->ptr_Pics = ptr_Pics;										// Передача ссылки на массив изображений в выходную структуру out.
	// Запуск цикла для считывания всех изображений.
	for (int i = 0; i < PicCount; i++) {
		// Вызов функции FileNameGenerator для генерации ссылки на входной файл (аргумент 'i').
		file = FileNameGenerator(i+1, 'i');
		ptr_Pics[i].ptr_lin = new Line[1000];						// Выделение памяти для массива пикселей.
		ptr_Pics[i].ptr_pal = new RGBQUAD[256];						// Выделение памяти для палитры.
		// Считывание структур bmp файла.
		fread(&ptr_Pics[i].File, sizeof(BITMAPFILEHEADER), 1, file);
		fread(&ptr_Pics[i].Info, sizeof(BITMAPINFOHEADER), 1, file);
		fread(ptr_Pics[i].ptr_pal, sizeof(RGBQUAD), 256, file);
		// Вычисление количества байтов, добавленных к строкам до длины кратной 4.
		ptr_Pics[i].PadBytes = (4 - (ptr_Pics[i].Info.biWidth % 4));
		if (ptr_Pics[i].PadBytes == 4)
			ptr_Pics[i].PadBytes = 0;
		// Считывание массива пикселей.
		for (int j = 0; j < ptr_Pics[i].Info.biHeight; j++) {
			fread(ptr_Pics[i].ptr_lin[j].ColourIndex, 1, ptr_Pics[i].Info.biWidth, file);
			// Программа отбрасывает пустые дополненные байты, для простоты использования команд. 
			fseek(file, ptr_Pics[i].PadBytes, SEEK_CUR);
		}
	}
	return;
}

/**
 * \brief Полная процедура ввода данных.
 * \details Функция введена для удобства и объединяет в себе функции ввода команд и изображений. 
 * \details Функция создаёт, заполняет и выводит переменную типа InputResult 
 * \return InputResult temp - структура содержащая результаты ввода данных.
 */
InputResult FullInputFunc() {
	// Инициализация выходной структуры.
	InputResult temp;
	InputCommands(&temp);
	InputPictures(&temp);
	return temp;
}

/**
 * \brief Вывод изображений.
 * \details Функция побитово выводит изображения из массива в файлы "outputxx.bmp".
 * \details Этапы работы функции:
 * - функция инициализирует файловую переменную.
 * - запуск цикла для вывода всех изображений.
 * \param int PicCount - Количество изображений, которые необходимо вывести.
 * \param Pic* ptr_Pics - Массив изображений.
 * \return void.
 */
void OutputFunc(int PicCount, Pic* ptr_Pics) {
	// Функция инициализирует файловую переменную.
	FILE* file;
	// Запуск цикла для вывода всех изображений.
	for (int i = 0; i < PicCount; i++) {
		// Вызов функции FileNameGenerator для генерации ссылки на выходной файл (аргумент 'o').
		file = FileNameGenerator(i + 1, 'o');
		// Вывод структур файла и массива пикселей.
		fwrite(&ptr_Pics[i].File, sizeof(BITMAPFILEHEADER), 1, file);
		fwrite(&ptr_Pics[i].Info, sizeof(BITMAPINFOHEADER), 1, file);
		fwrite(ptr_Pics[i].ptr_pal, sizeof(RGBQUAD), 256, file);
		for (int j = 0; j < ptr_Pics[0].Info.biHeight; j++) {
			fwrite(ptr_Pics[i].ptr_lin[j].ColourIndex, 1, ptr_Pics[i].Info.biWidth, file);
			// Добавление байтов к строкам до кратности 4-м.
			fwrite(&ZeroBite, 1, ptr_Pics[0].PadBytes, file);
		}
	}
	return;
}