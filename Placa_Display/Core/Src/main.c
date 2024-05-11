/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ST7735\st7735.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define FonteQuad Font_16x26
#define DeslocX (2)
#define DeslocY (-3)
#define botEsquerda !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_9)
#define botDireita  !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_11)
#define botCima     !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_12)
#define botBaixo    !HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_10)
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
SPI_HandleTypeDef hspi1;

/* USER CODE BEGIN PV */
short cursorX = 1;
short cursorY = 1;
short pontosX = 0;
short pontosO = 0;
char* jogador = "x";
short matriz[3][3];
char buffer [10];
char buffer2 [10];
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_SPI1_Init(void);
/* USER CODE BEGIN PFP */
void desenhaQuad (short linha, short coluna, const char* chr);
void piscaCursor();
void cursorParaEsquerda();
void cursorParaDireita ();
void alternaJogador();
void fimJogada();
void consertaTabuleiro();
void fimRodada(short result);
void checarVitoria();
void novaRodada();
void zerarMatriz();
void atualizarPontos();
void fimDeJogo();
int sprintf(char *str, const char *format, ...);
/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_SPI1_Init();
  /* USER CODE BEGIN 2 */
  ST7735_Init();
  ST7735_FillScreen(WHITE);
  novaRodada();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  piscaCursor();
	  if (botEsquerda) {
	 cursorParaEsquerda();
	  }

	  if (botDireita) {
	  cursorParaDireita();
	  }

	  if (botBaixo) {
	  fimJogada();
	  HAL_Delay(300);
	  }

	  if (botCima) {
	  fimJogada();
	  HAL_Delay(300);
	  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI_DIV2;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL16;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief SPI1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_SPI1_Init(void)
{

  /* USER CODE BEGIN SPI1_Init 0 */

  /* USER CODE END SPI1_Init 0 */

  /* USER CODE BEGIN SPI1_Init 1 */

  /* USER CODE END SPI1_Init 1 */
  /* SPI1 parameter configuration*/
  hspi1.Instance = SPI1;
  hspi1.Init.Mode = SPI_MODE_MASTER;
  hspi1.Init.Direction = SPI_DIRECTION_1LINE;
  hspi1.Init.DataSize = SPI_DATASIZE_8BIT;
  hspi1.Init.CLKPolarity = SPI_POLARITY_LOW;
  hspi1.Init.CLKPhase = SPI_PHASE_1EDGE;
  hspi1.Init.NSS = SPI_NSS_SOFT;
  hspi1.Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_32;
  hspi1.Init.FirstBit = SPI_FIRSTBIT_MSB;
  hspi1.Init.TIMode = SPI_TIMODE_DISABLE;
  hspi1.Init.CRCCalculation = SPI_CRCCALCULATION_DISABLE;
  hspi1.Init.CRCPolynomial = 10;
  if (HAL_SPI_Init(&hspi1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN SPI1_Init 2 */

  /* USER CODE END SPI1_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(ST7735_CS_GPIO_Port, ST7735_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, ST7735_DC_Pin|ST7735_RES_Pin|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6, GPIO_PIN_RESET);

  /*Configure GPIO pin : ST7735_CS_Pin */
  GPIO_InitStruct.Pin = ST7735_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(ST7735_CS_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pins : ST7735_DC_Pin ST7735_RES_Pin PB3 PB4
                           PB5 PB6 */
  GPIO_InitStruct.Pin = ST7735_DC_Pin|ST7735_RES_Pin|GPIO_PIN_3|GPIO_PIN_4
                          |GPIO_PIN_5|GPIO_PIN_6;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pins : PA9 PA10 PA11 PA12 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_10|GPIO_PIN_11|GPIO_PIN_12;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */
void desenhaQuad (short linha, short coluna, const char* chr) {
	short posX = 50 + 20 * (linha - 1);
	short posY = 9 + 20 * (coluna - 1 );
	  ST7735_WriteString(posX + DeslocX, posY + DeslocY, chr , FonteQuad, BLACK, WHITE);
	  consertaTabuleiro();
}
void consertaTabuleiro () {
	  ST7735_DrawLine(50, 29, 110, 29, BLACK);
	  ST7735_DrawLine(50, 49, 110, 49, BLACK);
}
void cursorParaDireita () {
	short i;
	for (i = 1; i <=9; i++) {
	cursorX++;
	if (cursorX == 4 && cursorY == 3) {
		cursorX = 1;
		cursorY = 1;
	} else if (cursorX == 4) {
		cursorX = 1;
	    cursorY++;
	}

	if (matriz[cursorX - 1][cursorY - 1] == 0)
		return;

	}
	fimRodada(0);
}
void cursorParaEsquerda () {
	for (;;) {
	cursorX--;
	if (cursorX == 0 && cursorY == 1) {
		cursorX = 3;
		cursorY = 3;
	} else if (cursorX == 0) {
		cursorX = 3;
		cursorY--;
	}

	if (matriz[cursorX - 1][cursorY - 1] == 0)
		break;

	}
}
void alternaJogador() {
	if (*jogador == 'x') {
		ST7735_WriteString(0, 0, "o:" , Font_11x18, BLACK, WHITE);
	   	jogador = "o";
	    } else {
	    ST7735_WriteString(0, 0, "x:" , Font_11x18, BLACK, WHITE);
	   	jogador = "x";
	    }
}
void piscaCursor() {
	while (1) {
		desenhaQuad (cursorX, cursorY, jogador);
		for (int x=0;x<20;x++)
		{
			HAL_Delay(50);
			if (botCima || botDireita || botEsquerda || botBaixo)
			{
				HAL_Delay(66);
				desenhaQuad (cursorX, cursorY, " ");
				return;
			}
		}
		desenhaQuad (cursorX, cursorY, " ");
		for (int x=0;x<20;x++)
		{
			HAL_Delay(50);
			if (botCima || botDireita || botEsquerda || botBaixo)
			{
				HAL_Delay(66);
				return;
			}
		}
	}
}
void fimJogada() {
	if (*jogador == 'x') {
		matriz[cursorX - 1][cursorY - 1] = 1;
	} else {
		matriz[cursorX - 1][cursorY - 1] = -1;
	}
	desenhaQuad (cursorX, cursorY, jogador);
	alternaJogador();
	cursorX = 0;
	cursorY = 1;
	cursorParaDireita();
	checarVitoria();
}
void checarVitoria() {
	short i = 0;
	for (i=0; i<=2; i++) {
		if (matriz[i][0] + matriz[i][1] + matriz[i][2] == 3) {
			ST7735_DrawLine(59 + 20 * i + DeslocX, 9 + DeslocY, 59 + 20 * i, 74, BLACK);
			fimRodada(1);
		} else if (matriz[i][0] + matriz[i][1] + matriz[i][2] == -3) {
			ST7735_DrawLine(59 + 20 * i + DeslocX, 9 + DeslocY, 59 + 20 * i, 74, BLACK);
			fimRodada(2);
		}
	}
	for (i=0; i<=2; i++) {
		if (matriz[0][i] + matriz[1][i] + matriz[2][i] == 3) {
			ST7735_DrawLine(45, 21 + 20 * i + DeslocY, 115, 21 + 20 * i + DeslocY, BLACK);
			fimRodada(1);
		} else if (matriz[0][i] + matriz[1][i] + matriz[2][i] == -3) {
			ST7735_DrawLine(45, 21 + 20 * i + DeslocY, 115, 21 + 20 * i + DeslocY, BLACK);
			fimRodada(2);
		}
	}

	if (matriz[0][0] + matriz[1][1] + matriz[2][2] == 3) {
		ST7735_DrawLine(48, 8, 111, 69, BLACK);
		fimRodada(1);
	} else if (matriz[0][0] + matriz[1][1] + matriz[2][2] == -3) {
		ST7735_DrawLine(48, 8, 111, 69, BLACK);
		fimRodada(2);
	}

	if (matriz[0][2] + matriz[1][1] + matriz[2][0] == 3) {
		ST7735_DrawLine(48, 69, 111, 8, BLACK);
		fimRodada(1);
	} else if (matriz[0][2] + matriz[1][1] + matriz[2][0] == -3) {
		ST7735_DrawLine(48, 69, 111, 8, BLACK);
		fimRodada(2);
	}
}
void fimRodada(short result) {
	switch (result) {
	case 0:
		break;
	case 1:
		pontosX++;
		jogador = "o";
		atualizarPontos();
		break;
	case 2:
		pontosO++;
		jogador = "x";
		atualizarPontos();
		break;
	}
	HAL_Delay(400);
	while (!botCima && !botBaixo && !botEsquerda && !botDireita) {}
	if (pontosX == 2 || pontosO == 2)
		fimDeJogo();
	zerarMatriz();
	novaRodada();
	cursorX = 0;
	cursorY = 1;
	cursorParaDireita();
}
void zerarMatriz() {
	short i = 0;
	short j = 0;
	for (i=0; i<=2; i++) {
		for (j=0; j<=2; j++) {
			matriz[i][j] = 0;
		}
	}
}
void atualizarPontos() {
	if (pontosX > 0)
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, 1);
	if (pontosX > 1)
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_5, 1);
	if (pontosO > 0)
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_3, 1);
	if (pontosO > 1)
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_4, 1);
	sprintf(buffer, "O:%d", pontosO);
	sprintf(buffer2, "X:%d", pontosX);
	ST7735_WriteString(159 - 7 * 3, 79 - 10 * 1, buffer , Font_7x10, BLACK, WHITE);
	ST7735_WriteString(159 - 7 * 3, 79 - 10 * 2, buffer2 , Font_7x10, BLACK, WHITE);
}
void novaRodada() {
	ST7735_FillScreen(WHITE);
	ST7735_DrawLine(50, 29, 110, 29, BLACK);
	ST7735_DrawLine(50, 49, 110, 49, BLACK);
	ST7735_DrawLine(70, 9, 70, 69, BLACK);
	ST7735_DrawLine(90, 9, 90, 69, BLACK);

	atualizarPontos();
	ST7735_WriteString(0, 0, jogador , Font_11x18, BLACK, WHITE);
	ST7735_WriteString(11, 0, ":" , Font_11x18, BLACK, WHITE);
}
void fimDeJogo() {
	ST7735_FillScreen(WHITE);
	ST7735_WriteString(34, 24, "Vencedor:" , Font_11x18, BLACK, WHITE);
	if (pontosX == 2) {
	    ST7735_WriteString(73, 43, "x" , Font_16x26, BLACK, WHITE);
	} else {
		ST7735_WriteString(73, 43, "o" , Font_16x26, BLACK, WHITE);
	}
	while (1) {}
}
/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
