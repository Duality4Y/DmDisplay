//init analog
void init_analog(void)
{
	//select 5v analog reference.
	ADMUX = (1<<REFS0);
	//enable adc and use a prescaler of 128
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
//adc returns a 10bit int so uint8_t isn't going to do it for us.
uint16_t adc_read(uint8_t ch)
{
	//only check for 3 first bits, or else we would select other options.
	ch &= 0x07;
	//clear first 3 bits before ORring, just incase.
	ADMUX = (ADMUX & 0xF8)|ch;
	//start conversion
	ADCSRA |= (1<<ADSC);
	//wait for it to complete.
	while(ADCSRA & (1<<ADSC));
	//return the value.
	return (ADC);
}
