package com.isp.service;

import com.isp.dto.ChlamydiaPatient;
import com.isp.dto.Questionnaire;
import com.isp.dto.QuestionnaireResponse;

import java.util.Optional;

public interface CheckFieldsPatientService {
    Optional<Questionnaire> checkFields(ChlamydiaPatient chlamydiaPatient, QuestionnaireResponse qr) throws NoSuchFieldException;
}
