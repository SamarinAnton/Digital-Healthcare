package com.isp.dto;

import lombok.Getter;
import lombok.Setter;

import java.util.List;

import static com.isp.constants.Endpoints.RESOURCE_TYPE_QUESTIONNAIRE;
import static com.isp.constants.Endpoints.STATUS_QUESTIONNAIRE;

@Getter
@Setter
public class Questionnaire {
    public static final String resourceType = RESOURCE_TYPE_QUESTIONNAIRE;
    public static final String status = STATUS_QUESTIONNAIRE;
    private String id;
    private String title;
    private List<Item> item;

    public Questionnaire(String title, List<Item> item) {
        this.title = title;
        this.item = item;
        this.id = CDSHooksCardsGeneral.stringQuestionnaireId();
    }
}
