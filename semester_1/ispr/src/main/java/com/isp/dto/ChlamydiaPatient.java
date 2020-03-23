package com.isp.dto;

import lombok.Getter;
import lombok.Setter;

import java.util.*;

@Getter
@Setter
public class ChlamydiaPatient extends Patient {
    private Boolean sexuallyActive;
    private String observedTime;
    private Boolean isObserved;
    private String testTime;
    private String result;
    private Boolean isChlamydiaTest;

    private List<Object> fields = new ArrayList<>(Collections.nCopies(8, null));

    public static Map<Integer, List<String>> questionsMap = new HashMap<>();

    static {
        questionsMap.put(0, Arrays.asList("What is the patient's gender?", "string"));
        questionsMap.put(1, Arrays.asList("What is the patient's birthday?", "string"));
        questionsMap.put(2, Arrays.asList("Is the patient sexually active?", "boolean"));
        questionsMap.put(3, Arrays.asList("Did the patient take a Chlamydia Screening test?", "boolean"));
        questionsMap.put(4, Arrays.asList("When did the patient take the Chlamydia Screening test?", "string"));
        questionsMap.put(5, Arrays.asList("What was the result of patient's Chlamydia Screening test?", "string"));
        questionsMap.put(6, Arrays.asList("Was the patient observed for sexually transmitted infection?", "boolean"));
        questionsMap.put(7, Arrays.asList("When was the patient observed for sexually transmitted infection?", "string"));
    }

    public ChlamydiaPatient(String gender, String birthDate, String id) {
        super(gender, birthDate, id);
    }

    public void fieldsToArray() {
        if (Objects.nonNull(gender)) {
            fields.set(0, gender);
        }

        if (!Objects.isNull(birthDate)) {
            fields.set(1, birthDate);
        }

        if (!Objects.isNull(sexuallyActive)) {
            fields.set(2, sexuallyActive);
        }

        if (!Objects.isNull(isChlamydiaTest)) {
            fields.set(3, isChlamydiaTest);
        }

        if (!Objects.isNull(testTime)) {
            fields.set(4, testTime);
        }

        if (!Objects.isNull(result)) {
            fields.set(5, result);
        }

        if (!Objects.isNull(isObserved)) {
            fields.set(6, isObserved);
        }

        if (!Objects.isNull(observedTime)) {
            fields.set(7, observedTime);
        }
    }

    public void arrayToFields() {
        gender = (String) fields.get(0);
        birthDate = (String) fields.get(1);
        sexuallyActive = (Boolean) fields.get(2);
        isChlamydiaTest = (Boolean) fields.get(3);
        testTime = (String) fields.get(4);
        result = (String) fields.get(5);
        isObserved = (Boolean) fields.get(6);
        observedTime = (String) fields.get(7);
    }
}