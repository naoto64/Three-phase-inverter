/*
 * File:   main.c
 * Author: naoto
 *
 * Created on 2023/03/25, 18:41
 */


#include <xc.h>

#pragma config FOSC = INTOSC    // 内部クロックを使用
#pragma config PLLEN = ON       // PLLを有効化
#pragma config WDTE = OFF       // ウォッチドッグタイマーは使用しない
#pragma config CP = OFF         // コードプロテクションは無効
#pragma config LVP = OFF        // LVPは使用しない

#define _XTAL_FREQ 32000000
#define IN1 PORTBbits.RB0
#define IN2 PORTBbits.RB1
#define IN3 PORTBbits.RB2
#define SD1 PORTBbits.RB3
#define SD2 PORTBbits.RB4
#define SD3 PORTBbits.RB5
#define DEADTIME 10 // デッドタイム（10us）

#define U_OUT_H LATAbits.LATA0
#define U_OUT_L LATAbits.LATA1
#define V_OUT_H LATAbits.LATA2
#define V_OUT_L LATAbits.LATA3
#define W_OUT_H LATAbits.LATA4
#define W_OUT_L LATAbits.LATA6

void __interrupt() isr(void) {
    if (TMR2IF == 1) {
        // U相
        if (SD1 == 1) { // SD1がHighの場合
            if (IN1 == 1) {    // IN1がHighの場合
                if (U_OUT_L == 1) {   // ローサイド側がONの場合
                    U_OUT_H = 0;
                    U_OUT_L = 0;
                }
                else {
                    U_OUT_H = 1;
                    U_OUT_L = 0;
                }
            }
            else {  // IN1がLowの場合
                if (U_OUT_H == 1) {   // ハイサイド側がONの場合
                    U_OUT_H = 0;
                    U_OUT_L = 0;
                }
                else {
                    U_OUT_H = 0;
                    U_OUT_L = 1;
                }
            }
        }
        else {
            U_OUT_H = 0;
            U_OUT_L = 0;
        }
        
        // V相
        if (SD2 == 1) { // SD2がHighの場合
            if (IN2 == 1) {    // IN2がHighの場合
                if (V_OUT_L == 1) {   // ローサイド側がONの場合
                    V_OUT_H = 0;
                    V_OUT_L = 0;
                }
                else {
                    V_OUT_H = 1;
                    V_OUT_L = 0;
                }
            }
            else {  // IN2がLowの場合
                if (V_OUT_H == 1) {   // ハイサイド側がONの場合
                    V_OUT_H = 0;
                    V_OUT_L = 0;
                }
                else {
                    V_OUT_H = 0;
                    V_OUT_L = 1;
                }
            }
        }
        else {
            V_OUT_H = 0;
            V_OUT_L = 0;
        }
        
        // W相
        if (SD3 == 1) { // SD3がHighの場合
            if (IN3 == 1) {    // IN3がHighの場合
                if (W_OUT_L == 1) {   // ローサイド側がONの場合
                    W_OUT_H = 0;
                    W_OUT_L = 0;
                }
                else {
                    W_OUT_H = 1;
                    W_OUT_L = 0;
                }
            }
            else {  // IN3がLowの場合
                if (W_OUT_H == 1) {   // ハイサイド側がONの場合
                    W_OUT_H = 0;
                    W_OUT_L = 0;
                }
                else {
                    W_OUT_H = 0;
                    W_OUT_L = 1;
                }
            }
        }
        else {
            W_OUT_H = 0;
            W_OUT_L = 0;
        }
        // TMR2割り込みフラグをクリアする
        PIR1bits.TMR2IF = 0;
    }
    return;
}

void main(void) {
    /* ---------- クロック関連処理 ---------- */
    OSCCON = 0b01110000;
    
    /* ---------- IOポートの設定 ---------- */
    ANSELA = 0;         // すべてをデジタルモードに設定
    ANSELB = 0;         // すべてをデジタルモードに設定
    TRISA = 0b00000000; // RA0～RA7を出力に設定
    TRISB = 0b00111111; // RB0～RB5を入力に設定（RB6～7は出力に設定）
    LATA = 0b00000000;  // ポートAをすべてLowに設定
    
    /* ---------- 割り込み関連設定 ---------- */
    T2CON = 0b00000100; // TMR2プリスケーラ値を1倍、ポストスケーラ値は1:1の設定
    PR2 = 32 / 4 * DEADTIME - 1;    // タイマーのカウント値を設定 (32MHz / 4 * 5us - 1)
    TMR2 = 0;   // タイマー2の初期化
    TMR2IF = 0; // タイマー2割込フラグを0にする
    TMR2IE = 1; // タイマー2割込みを許可する
    PEIE = 1;   // 周辺装置割り込みを許可する
    GIE = 1;    // グローバル割り込みを許可する

    /* ---------- メインループ ---------- */
    while (1) {
    }
    return;
}