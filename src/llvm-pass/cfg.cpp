#include <stdio.h>

int main() {
    int input;
    printf("Enter an integer: ");
    scanf("%d", &input);

    int state = 0; // 초기 상태 설정
    int result = 0;

    while (state != -1) {
        switch (state) {
            case 0:
                if (input > 0 && input <= 10) {
                    state = 1; // 상태 전환
                } else if (input > 10) {
                    state = 2; // 상태 전환
                } else {
                    state = -1; // 종료 상태
                }
                break;

            case 1:
                for (int i = 0; i < input; i++) {
                    result += i;
                }
                state = -1; // 종료 상태
                break;

            case 2:
                while (input > 10) {
                    result += input;
                    input--;
                }
                state = -1; // 종료 상태
                break;

            default:
                state = -1; // 안전장치로서의 종료 상태
                break;
        }
    }

    printf("Result: %d\n", result);
    return 0;
}

