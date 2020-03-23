package com.isp.dto;

import lombok.Getter;
import lombok.Setter;

import static com.isp.dto.CDSHooksCardsGeneral.stringLinkId;

@Getter
@Setter
public class Item {
    private String linkId;
    private String text;
    private String type;

    public Item(String text, String type) {
        this.linkId = stringLinkId();
        this.text = text;
        this.type = type;
    }
}
