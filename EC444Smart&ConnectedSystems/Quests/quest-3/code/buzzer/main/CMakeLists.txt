idf_component_register(SRCS "buzzer_main.c"
                    INCLUDE_DIRS "."
		    PRIV_REQUIRES esp_driver_dac esp_adc)
