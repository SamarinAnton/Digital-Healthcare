package com.isp.service;

import com.isp.dto.Card;
import com.isp.dto.ChlamydiaPatient;

public interface ChlamydiaService {

    Card makeDecision(ChlamydiaPatient request);
}
