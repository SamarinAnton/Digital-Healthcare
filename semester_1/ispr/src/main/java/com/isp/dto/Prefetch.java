package com.isp.dto;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.Setter;

@Getter
@Setter
@AllArgsConstructor
public class Prefetch {
    private Patient patient;
    private Questionnaire questionnaire;
    private QuestionnaireResponse questionnaireResponse;

    public Prefetch(Questionnaire questionnaire) {
        this.questionnaire = questionnaire;
    }
}