package com.isp.dto;

import lombok.Getter;
import lombok.Setter;

import static com.isp.constants.Endpoints.INDICATOR_FOR_CARD;

@Getter
@Setter
public class Card {
    public static final String indicator = INDICATOR_FOR_CARD;
    private Object source;
    private String summary;
    private String detail;

    public Card(String summary, String detail) {
        this.summary = summary;
        this.detail = detail;
        this.source = "ChlamydiaScreeningCDS.cql";
    }
}
