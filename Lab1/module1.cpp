#include "module1.h"
#include "module1_step1.h"
#include "module1_step2.h"

// Головна функція, яка керує перемиканням між двома окремими модулями вікон
bool ShowDialogModule1(HWND hWndParent) {
    int step = 1; // Починаємо з першого вікна
    
    while (step != 0 && step != 3) {
        if (step == 1) {
            // Викликаємо функцію з модуля module1_step1
            INT_PTR res = ShowDialogStep1(hWndParent);
            if (res == 1) step = 2; 
            else step = 0; 
        } else if (step == 2) {
            // Викликаємо функцію з модуля module1_step2
            INT_PTR res = ShowDialogStep2(hWndParent);
            if (res == 2) step = 1; 
            else if (res == 3) step = 3;
            else step = 0; 
        }
    }
    
    return (step == 3); // Повертає true, якщо пройшли обидва вікна
}