package com.isp.dto;

import java.util.Collections;
import java.util.concurrent.atomic.AtomicInteger;

public class CDSHooksCardsGeneral {
    private static int questionnaireId = 0;
    private static AtomicInteger linkId = new AtomicInteger(0);

    public static String stringQuestionnaireId() {
        return String.format("qId%s", String.valueOf(questionnaireId++));
    }

    public static String stringLinkId() {
        // TODO atomic variable
        return String.format("lId%s", String.valueOf(linkId.getAndAdd(1)));
    }
    public static Cards cardsResponse(String summary, String detail) {
        return new Cards(Collections.singletonList(new Card(summary, detail)));
    }
}