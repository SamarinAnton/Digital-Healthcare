package com.isp.dto;

import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

import java.util.Date;
import java.util.List;

import static com.isp.dto.CDSHooksCardsGeneral.stringQuestionnaireId;

@NoArgsConstructor
@Getter
@Setter
public class QuestionnaireResponse {
    private String resourceType;
    private String status;
    private String id;
    private String questionnaire;
    private Date authored;
    public List<AnswerItem> item;

    public QuestionnaireResponse(List<AnswerItem> item) {
        this.item = item;
        this.id = stringQuestionnaireId();
    }
}
