package com.isp.dto;

import lombok.Getter;
import lombok.Setter;

import java.util.ArrayList;
import java.util.List;

@Getter
@Setter
public class AnswerItem {
    private String linkId;
    private String text;
    private List<Answer> answer;

    public AnswerItem(String question, Answer answer) {
        this.linkId = CDSHooksCardsGeneral.stringLinkId();
        this.text = question;
        this.answer = new ArrayList<>();
        this.answer.add(answer);
    }
}
