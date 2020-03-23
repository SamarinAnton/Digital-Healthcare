package com.isp.dto;

import lombok.AllArgsConstructor;
import lombok.Getter;
import lombok.NoArgsConstructor;
import lombok.Setter;

@NoArgsConstructor
@AllArgsConstructor
@Getter
@Setter
public class Request {
    private String hook;
    private String hookInstance;
    private String fhirServer;
    private String user;
    private Prefetch prefetch;
    private Card card;
}
