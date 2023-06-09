//constants used for registers and commands
uint8_t tx_command  = 0b10100000;
uint8_t rx_command = 0b01100001;
uint8_t nop_command = 0b11111111;
#define W_REGISTER_command 0x0
#define CONFIG 0x0
#define EN_AA 0x1
#define EN_RXADDR 0x2
#define SETUP_AW 0x3
#define SETUP_RETR 0x4
uint8_t CONFIG_command = W_REGISTER_command + CONFIG;
uint8_t CONFIG_value = 0b00000010;
uint8_t EN_AA_command = W_REGISTER_command + EN_AA;
uint8_t EN_AA_value = 0b00000000;
uint8_t EN_RXADDR_command = W_REGISTER_command + EN_RXADDR;
uint8_t EN_RXADDR_value = 0b00000000;
uint8_t SETUP_AW_command = W_REGISTER_command + SETUP_AW;
uint8_t SETUP_AW_value = 0b00000000;
uint8_t SETUP_RETR_command = W_REGISTER_command + SETUP_RETR;
uint8_t SETUP_RETR_value = 0b00000000;
extern SPI_HandleTypeDef hspi1;

void nrf24_CSN_Enable(void){
	HAL_GPIO_WritePin(SPI_CSN_Port, SPI_CSN_Pin, GPIO_PIN_SET);
}

void nrf24_CSN_Disable(void){
	HAL_GPIO_WritePin(SPI_CSN_Port, SPI_CSN_Pin, GPIO_PIN_RESET);
}

//Switching from STANDBY-1 mode to RX or TX mode
void nrf24_RX_TX_Enable(void){
	HAL_GPIO_WritePin(SPI_CE_Port, SPI_CE_Pin, GPIO_PIN_RESET);
}

//Switching from RX or TX mode to STANDBY-1 mode
void nrf24_RX_TX_Disable(void){
	HAL_GPIO_WritePin(SPI_CE_Port, SPI_CE_Pin, GPIO_PIN_SET);
}

//Transmitting data wirelessly
void nrf24_Transmit(uint8_t* pData){
	nrf24_CSN_Enable();
	HAL_SPI_Transmit(&hspi1,&tx_command,1,0);
	HAL_SPI_Transmit(&hspi1,pData+8,1,0);
	HAL_SPI_Transmit(&hspi1,pData,1,0);
	nrf24_CSN_Disable();
}

//Receiving wireless data
void nrf24_Receive(uint8_t* pData){
	nrf24_CSN_Enable();
	HAL_SPI_Transmit(&hspi1,&rx_command,1,0);
	HAL_SPI_Receive(&hspi1,pData+8,1,0);
	HAL_SPI_Receive(&hspi1,pData,1,0);
	nrf24_CSN_Disable();
}

//Writing in the nrf24 register
void nrf24_W_REGISTER(uint8_t reg, uint8_t value){
	nrf24_CSN_Enable();
	HAL_SPI_Transmit(&hspi1, &reg, 1, 0);
	HAL_SPI_Transmit(&hspi1, &value, 1, 0);
	nrf24_CSN_Disable();
}

//Reading the nrf24 registers
void nrf24_R_REGISTER(uint8_t reg, uint8_t* pData){
	nrf24_CSN_Enable();
	HAL_SPI_Transmit(&hspi1, &reg, 1, 0);
	HAL_SPI_Receive(&hspi1,pData,1,0);
	nrf24_CSN_Disable();
}

// Reading status
void nrf24_STATUS(uint8_t* pData){
	nrf24_CSN_Enable();
	HAL_SPI_TransmitReceive(&hspi1 ,&nop_command, pData,1,0);
	nrf24_CSN_Disable();
}

//Controller Initialization
void nrf24_Init_Controller(){
	nrf24_W_REGISTER(EN_AA_command,EN_AA_value);
	nrf24_W_REGISTER(CONFIG_command,CONFIG_value);
	nrf24_W_REGISTER(EN_RXADDR_command,EN_AA_value);
	nrf24_W_REGISTER(SETUP_AW_command,SETUP_AW_value);
	nrf24_W_REGISTER(SETUP_RETR_command,SETUP_RETR_value);
	nrf24_RX_TX_Enable();
}

//Console Initialization
void nrf24_Init_plug(){
	nrf24_W_REGISTER(EN_AA_command,EN_AA_value);
	nrf24_W_REGISTER(CONFIG_command,CONFIG_value);
	nrf24_W_REGISTER(EN_RXADDR_command,EN_AA_value);
	nrf24_W_REGISTER(SETUP_AW_command,SETUP_AW_value);
	nrf24_W_REGISTER(SETUP_RETR_command,SETUP_RETR_value);
	nrf24_RX_TX_Enable();
}

