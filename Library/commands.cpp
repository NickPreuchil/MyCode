/*****************************************************************//**
 * \file  commands.cpp
 * \brief Файл реализации функций, выполняющих операции над изображениями.
 * \author Преучиль Н.А.
 * \date   12 January 2021
 *********************************************************************/
#include "commands.h"


/**
 * \brief Поворот изображений.
 * \details В зависимости от переданного аргумента, функция может повернуть изображение на 0, 90, 180, 270 и 360 градусов по часовой стрелке.
 * \details Функция также вносит изменения в структуру изображения, поскольку тот факт, что мы меняем местами
 * ширину и высоту изображения, может повлиять на то количество байтов, которое нужно добавить к каждой линии развёртки до длины кратной 4.
 * \details Алгоритм реализуется путём прохода по массиву пикселей старого и нового изображения по разным траекториям (подробнее в коде программы).
 * \details Этапы работы функции:
 * - выделение памяти для нового массива пикселей.
 * - выбор режима работы в зависимости от переданного аргумента.
 * - перезапись пикселей.
 * - при повороте на 90 и 270 необходимо учесть изменение разрешения (подробнее выше)
 * - освобождение выделенной ранее памяти для старого массива пикселей.
 * - передача новой ссылки на массив пикселей в структуру изображения.
 * \param Pic* Picture - указатель на структуру изображения, которое нужно повернуть.
 * \param char* Arg - строка с аргументом функции (числовое значение ANGLE ε {0, 90, 180, 270, 360})
 * \return void.
 */
void rotate(Pic* Picture, char* Arg){
	int Angle = atoi(Arg);									// Получение численного значения строки - аргумента

	// Выделение памяти для нового массива пикселей.
	Line* ptr_linTemp;
	ptr_linTemp = new Line[1000];
	// Выбор режима работы в зависимости от переданного аргумента.
	switch (Angle) {
		case 180: {
			// Перезапись пикселей.
			// Строки и пиксели в каждой строке записываются в обратном порядке. 
			for (int i = 0; i < Picture->Info.biHeight; i++) {
				for (int j = 0; j < Picture->Info.biWidth; j++) {
					ptr_linTemp[i].ColourIndex[j] = Picture->ptr_lin[Picture->Info.biHeight - 1 - i].ColourIndex[Picture->Info.biWidth - 1 - j];
				}
			}
			// Освобождение выделенной ранее памяти для старого массива пикселей.
			delete[] Picture->ptr_lin;
			// Передача новой ссылки на массив пикселей в структуру изображения.
			Picture->ptr_lin = ptr_linTemp;
			break;
		}
		case 90: {
			// Перезапись пикселей.
			// Элементы строк изначального массива с индексом (Ширина рисунка - i) становятся i-ми строками нового массива.
			for (int i = 0; i < Picture->Info.biWidth; i++) {
				for (int j = 0; j < Picture->Info.biHeight; j++) {
					ptr_linTemp[i].ColourIndex[j] = Picture->ptr_lin[j].ColourIndex[Picture->Info.biWidth - 1 - i];
				}
			}
			// Изменение структуры изображения 
			// 1. Высота станвится шириной и наоборот.
			int Temp;
			Temp = Picture->Info.biWidth;
			Picture->Info.biWidth = Picture->Info.biHeight;
			Picture->Info.biHeight = Temp;
			// 2. Рассчёт количества доп. байтов.
			Picture->PadBytes = (4 - (Picture->Info.biWidth % 4));
			if (Picture->PadBytes == 4)
				Picture->PadBytes = 0;
			// 3. Нахождения объёма массива пикселей и общего объёма занимаемого файлом (в байтах)
			Picture->Info.biSizeImg = Picture->Info.biWidth * Picture->Info.biHeight + Picture->PadBytes * Picture->Info.biHeight;
			Picture->File.bfSize = Picture->Info.biSizeImg + Picture->File.bfOffBit;
			// Освобождение выделенной ранее памяти для старого массива пикселей.
			delete[] Picture->ptr_lin;
			// Передача новой ссылки на массив пикселей в структуру изображения.
			Picture->ptr_lin = ptr_linTemp;
			break;
		}
		case 270: {
			// Перезапись пикселей.
			// i-е элементы строк изначального массива становятся i-ми строками нового массива.
			for (int i = 0; i < Picture->Info.biWidth; i++) {
				for (int j = 0; j < Picture->Info.biHeight; j++) {
					ptr_linTemp[i].ColourIndex[j] = Picture->ptr_lin[Picture->Info.biHeight - 1 - j].ColourIndex[i];
				}
			}
			// Изменение структуры изображения 
			// 1. Высота становится шириной и наоборот.
			int Temp;
			Temp = Picture->Info.biWidth;
			Picture->Info.biWidth = Picture->Info.biHeight;
			Picture->Info.biHeight = Temp;
			// 2. Рассчёт количества доп. байтов.
			Picture->PadBytes = (4 - (Picture->Info.biWidth % 4));
			if (Picture->PadBytes == 4)
				Picture->PadBytes = 0;
			// 3. Нахождения объёма массива пикселей и общего объёма занимаемого файлом (в байтах)
			Picture->Info.biSizeImg = Picture->Info.biWidth * Picture->Info.biHeight + Picture->PadBytes * Picture->Info.biHeight;
			Picture->File.bfSize = Picture->Info.biSizeImg + Picture->File.bfOffBit;
			// Освобождение выделенной ранее памяти для старого массива пикселей.
			delete[] Picture->ptr_lin;
			// Передача новой ссылки на массив пикселей в структуру изображения.
			Picture->ptr_lin = ptr_linTemp;
			break;
		}
		case 360:{
			// С изображением ничего не происходит.
			// Очищаем выделенную память за ненадобностью.
			delete[] ptr_linTemp;
			break;
		}
		case 0: {
			// С изображением ничего не происходит.
			// Очищаем выделенную память за ненадобностью.
			delete[] ptr_linTemp;
			break;
		}
		default: {
			assert(false);														// Остановка программы в случае, если switch не обработал аргумент.
		}
	}
	
	
	return;
}

/**
 * \brief Изменение яркости пикселей.
 * \details Функция изменяет яркость изображения с помощю побитового сдвига численного значения каждого пикселя. 
 * \details Биты, вышедшие за границу однобайтового индекса пикселя отбрасываются.
 * \details Работа функции заключается в проходе по каждому элементу массива и операции побайтого сдвига на f разрядов влево или вправо в зависимости от знака f:
 * - при f > 0 - сдвиг на f разрядов влево.
 * - при f < 0 - сдвиг на f разрядов вправо.
 * \param Pic* Picture - указатель на структуру изображения, яркость которого нужно изменить.
 * \param char* Arg - строка с аргументом функции (-8 ≤ f ≤ 8).
 * \return void.
 */
void bright(Pic* Picture, char* Arg) {
	int f = atoi(Arg);						// Получение численного значения аргумента.
	// Простой проход по массиву с помошью вложенного цикла.
	if (f > 0) {
		for (int i = 0; i < Picture->Info.biHeight; i++) {
			for (int j = 0; j < Picture->Info.biWidth; j++) {
				// Операция сдвига.
				Picture->ptr_lin[i].ColourIndex[j] = Picture->ptr_lin[i].ColourIndex[j] << f;
			}
		}
	}
	else {
		for (int i = 0; i < Picture->Info.biHeight; i++) {
			for (int j = 0; j < Picture->Info.biWidth; j++) {
				// Операция сдвига.
				Picture->ptr_lin[i].ColourIndex[j] = Picture->ptr_lin[i].ColourIndex[j] >> -f;
			}
		}
	}
	return;
}

/**
 * \brief Зеркальное отражение изображение.
 * \details Функция отражает изображение относительно вертикальной или горизонтальной оси, в зависимости от переданного аргумента:
 * \details Работа алогритма заключается в создании и заполнении нового массива пикселями из старого, с помошью прохода по старому массиву в другом порядке, нежели при заполнении.
 * \details Этапы работы функции:
 * - выделение памяти для нового массива пикселей.
 * - выбор, относительно какой оси будет происходить отражение.
 * - перезапись пикселей. 
 * - освобождение выделенной ранее памяти для старого массива пикселей.
 * - передача новой ссылки на массив пикселей в структуру изображения.
 * \param Pic* Picture - указатель на структуру изображения, которое нужно отразить.
 * \param char* Arg - строка с аргументом функции (Символы 'V' или 'H').
 * \details Значение аргументов
 * - 'V' - отражение относительно вертикальной оси.
 * - 'H' - отражение относительно горизонтальной оси.
 * \return void.
 */
void flip(Pic* Picture, char* Arg) {
	char Dir = Arg[0];
	// Выделение памяти для нового массива пикселей.
	Line* ptr_linTemp;
	ptr_linTemp = new Line[1000];
	// Выбор
	switch (Dir)
	{
	// В случае с отражением относительно горизонтальной оси, линии изначального массива пикселей записываются в новый массив в обратном порядке, 
	// а пиксели в строках записываются в прямом порядке.
	case 'H': {
		for (int i = 0; i < Picture->Info.biHeight; i++) {
			ptr_linTemp[i] = Picture->ptr_lin[Picture->Info.biHeight - 1 - i];
		}
		// Освобождение выделенной ранее памяти для старого массива пикселей.
		delete[] Picture->ptr_lin;
		break;
	}
	// При отражении отноительно вертикальной оси, строки из изначального массива записываются в прямом порядке, а пиксели в обратном. 
	case 'V': {
		for (int i = 0; i < Picture->Info.biHeight; i++) {
			for (int j = 0; j < Picture->Info.biWidth; j++) {
				ptr_linTemp[i].ColourIndex[j] = Picture->ptr_lin[i].ColourIndex[Picture->Info.biWidth - 1 - j];
			}
		}
		// Освобождение выделенной ранее памяти для старого массива пикселей.
		delete[] Picture->ptr_lin;
		break;
	}
	default:
		assert (false);														// Остановка программы в случае, если switch не обработал аргумент.
	}
	Picture->ptr_lin = ptr_linTemp;
	return;
}

/**
 * \brief Уменьшение количества пикселей.
 * \details Функция уменьшает количество пикселей в V раз по вертикали и в H раз по горизонтали.
 * \warning Операция выполняется только в том случае, если количество пикселей по вертикали и горизонтали
 * одновременно кратны V и H соответственно.
 * \details Уменньшение количества пикселей заключается по сути в пропуске пикселей при проходе по массиву. 
 * Алгоритм реализуется путём прохода по изначальному массиву с шагом V по вертикали и H по горизонтали и записи пикселей в новый массив, 
 * проход по которому в свою очередь идёт совершенно обычный.
 * \details Функция также вносит изменения в структуры BITMAPFILEHEADER и BITMAPINFOHEADER, поскольку разрешение файла изменилось.
 * \details Этапы работы функции:
 * - проверка условий.
 * - выделение памяти для нового массива пикселей.
 * - перезапись пикселей.
 * - изменение структуры изображения в связи с изменением разрешения.
 * - освобождение выделенной ранее памяти для старого массива пикселей.
 * - передача новой ссылки на массив пикселей в структуру изображения.
 * \param Pic* Picture - указатель на структуру изображения, количество пикселей которого нужно уменьшить.
 * \param char* Arg - строка с аргументом функции ("V H"). 1 ≤ V,H ≤ 10;
 * \return void.
 */
void downscale(Pic* Picture, char* Arg) {
	// Получение числового значения символов - аргументов (по таблице ASCII)
	int VArg = Arg[0] - 48;		// Вертикальный делитель.
	int HArg = Arg[2] - 48;		// Горизонтальный делитель.
	// Проверка условий.
	if ((Picture->Info.biHeight % VArg == 0) && (Picture->Info.biWidth % HArg == 0)) {
		// Выделение памяти для нового массива пикселей.
		Line* ptr_linTemp;
		ptr_linTemp = new Line[1000];
		// Перезапись пикселей.
		for (int i = 0; i < Picture->Info.biHeight / VArg; i++) {
			for (int j = 0; j < Picture->Info.biWidth / HArg; j++) {
				// Видно, как именно реализован проход по массиву с различным шагом:
				// Индекс пикселей в строке нового массива меняется как j, а в старом массиве как (j * Вертикальный аргумент)
				ptr_linTemp[Picture->Info.biHeight / VArg - 1 - i].ColourIndex[j] = Picture->ptr_lin[Picture->Info.biHeight - 1 - i * VArg].ColourIndex[j * HArg];
			}
		}
		// Изменение структуры изображения в связи с изменением разрешения.
		// 1. wВысота становится шириной и наоборот
		Picture->Info.biHeight /= VArg;
		Picture->Info.biWidth /= HArg;
		// 2. Рассчёт количества доп. байтов.
		Picture->PadBytes = (4 - (Picture->Info.biWidth % 4));
		if (Picture->PadBytes == 4)
			Picture->PadBytes = 0;
		// 3. Нахождения объёма массива пикселей и общего объёма занимаемого файлом (в байтах)
		Picture->Info.biSizeImg = Picture->Info.biWidth * Picture->Info.biHeight + Picture->PadBytes * Picture->Info.biHeight;
		Picture->File.bfSize = Picture->Info.biSizeImg + Picture->File.bfOffBit;
		// Освобождение выделенной ранее памяти для старого массива пикселей.
		delete[] Picture->ptr_lin;
		// Передача новой ссылки на массив пикселей в структуру изображения.
		Picture->ptr_lin = ptr_linTemp;
	}
	else {
	#ifdef DEBUG
		cout << "downscale command skipped" << endl;										// Контроль срабатывания команд со входными условиями.
	#endif DEBUG
	}
	return;
}

/**
 * \brief Увеличение количества пикселей.
 * \details Функция увеличивает количество пикселей в V раз по вертикали и в H раз по горизонтали.
 * \Warning Операция выполняется только в том случае, если при увеличении,
 * количество пикселей по вертикали и горизонтали одновременно не привышает максимальное значение (В рамках ИЗ - 1000 пикселей)
 * \details Алгоритм заключается в повторении каждой строки входящего изображения V раз и каждого пикселя строки H раз. 
 * \details Функция также вносит изменения в структуры BITMAPFILEHEADER и BITMAPINFOHEADER, поскольку разрешение файла изменилось.
 * \details Этапы работы функции:
 * - проверка условий.
 * - выделение памяти для нового массива пикселей.
 * - перезапись пикселей.
 * - изменение структуры изображения в связи с изменением разрешения.
 * - освобождение выделенной ранее памяти для старого массива пикселей.
 * - передача новой ссылки на массив пикселей в структуру изображения.
 * \param Pic* Picture - указатель на структуру изображения, количество пикселей которого нужно увеличить.
 * \param char* Arg - строка с аргументом функции ("V H"). 1 ≤ V,H ≤ 10;
 * \return void.
 */
void upscale(Pic* Picture, char* Arg) {
	// Получение числового значения символов - аргументов (по таблице ASCII)
	int VArg = Arg[0] - 48;		// Вертикальный делитель.
	int HArg = Arg[2] - 48;		// Горизонтальный делитель.
	// Проверка условий.
	if ((Picture->Info.biHeight * VArg <= 1000) && (Picture->Info.biWidth * HArg <= 1000)) {
		// Выделение памяти для нового массива пикселей.
		Line* ptr_linTemp;
		ptr_linTemp = new Line[1000];
		// Перезапись пикселей.
		// Цикл останавливается на конкретной строке старого массива.
		for (int i = 0; i < Picture->Info.biHeight; i++) {
			// Запускается цикл, который будет повторять одну и ту же строку V раз. Индексы специально подобраны для удобства передачи в индексы. 
			for (int TempV = i * VArg; TempV < (i + 1) * VArg; TempV++) {
				// Третий цикл останавливается на конкретном пикселе строки. 
				for (int j = 0; j < Picture->Info.biWidth; j++) {
					// Далее запускается цикл, который будет повторять этот пиксель H раз. 
					for (int TempH = j * HArg; TempH < (j + 1) * HArg; TempH++) {
						// Цикл четвёртой степени вложенности может навести на мысли о долгой работе алгоритма, но в любом случае алгоритм будет заполнять матрицу не более 1000 на 1000 пикселей.
						ptr_linTemp[TempV].ColourIndex[TempH] = Picture->ptr_lin[i].ColourIndex[j];
					}
				}
			}
		}
		// Изменение структуры изображения в связи с изменением разрешения.
		Picture->Info.biHeight *= VArg;
		Picture->Info.biWidth *= HArg;
		Picture->PadBytes = (4 - (Picture->Info.biWidth % 4));
		if (Picture->PadBytes == 4)
			Picture->PadBytes = 0;
		Picture->Info.biSizeImg = Picture->Info.biWidth * Picture->Info.biHeight + Picture->PadBytes * Picture->Info.biHeight;
		Picture->File.bfSize = Picture->Info.biSizeImg + Picture->File.bfOffBit;
		// Освобождение выделенной ранее памяти для старого массива пикселей.
		delete[] Picture->ptr_lin;
		// Передача новой ссылки на массив пикселей в структуру изображения.
		Picture->ptr_lin = ptr_linTemp;
	}
	else {
	#ifdef DEBUG
		cout << "upscale command skipped" << endl;										// Контроль срабатывания команд со входными условиями.
	#endif DEBUG
	}
	return;
}

void crop(Pic* Picture, char* Arg){
	int delta = atoi(Arg);
	Line* ptr_linTemp;
	ptr_linTemp = new Line[1000];
	for (int i = delta; i < Picture->Info.biHeight - delta; i++) {
		for (int j = delta; j < Picture->Info.biWidth - delta; j++) {
			ptr_linTemp[i - delta].ColourIndex[j - delta] = Picture->ptr_lin[i].ColourIndex[j];
		}
	}
	Picture->Info.biHeight -= 2 * delta;
	Picture->Info.biWidth -= 2 * delta;
	Picture->PadBytes = (4 - (Picture->Info.biWidth % 4));
	if (Picture->PadBytes == 4)
		Picture->PadBytes = 0;
	Picture->Info.biSizeImg = Picture->Info.biWidth * Picture->Info.biHeight + Picture->PadBytes * Picture->Info.biHeight;
	Picture->File.bfSize = Picture->Info.biSizeImg + Picture->File.bfOffBit;
	delete[] Picture->ptr_lin;
	Picture->ptr_lin = ptr_linTemp;
	return;
}
