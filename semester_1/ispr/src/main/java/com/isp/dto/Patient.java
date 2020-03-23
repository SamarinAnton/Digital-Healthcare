package com.isp.dto;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

import java.time.LocalDate;
import java.time.Period;
import java.time.format.DateTimeFormatter;

import static com.isp.constants.Endpoints.PATIENT_RESOURCE_TYPE;

@AllArgsConstructor
@Getter
@Setter
public class Patient {
    public static final String resourceType = PATIENT_RESOURCE_TYPE;
    private static boolean active = true;
    protected String gender;
    protected String birthDate;
    private String id;

    public int age() {
        return Period.between(LocalDate.parse(birthDate, DateTimeFormatter.ofPattern("yyyy-MM-dd")), LocalDate.now()).getYears();
    }
}
