package com.isp.controller;

import com.isp.constants.ApiConstant;
import com.isp.dto.*;
import com.isp.service.CheckFieldsPatientService;
import com.isp.service.ChlamydiaService;
import com.wordnik.swagger.annotations.ApiOperation;
import com.wordnik.swagger.annotations.ApiParam;
import lombok.SneakyThrows;
import org.springframework.beans.factory.annotation.Autowired;
import org.springframework.http.HttpStatus;
import org.springframework.http.ResponseEntity;
import org.springframework.web.bind.annotation.PostMapping;
import org.springframework.web.bind.annotation.RequestBody;
import org.springframework.web.bind.annotation.RequestMapping;
import org.springframework.web.bind.annotation.RestController;

import java.util.Map;
import java.util.Optional;
import java.util.concurrent.ConcurrentHashMap;

@RestController
@RequestMapping(ApiConstant.API_V_1 + "/chlamydiaTest")
public class ChlamydiaController {

    private Map<String, ChlamydiaPatient> chlamydiaPatientMap = new ConcurrentHashMap<>();
    private final ChlamydiaService chlamydiaService;
    private final CheckFieldsPatientService checkFieldsPatientService;

    @Autowired
    public ChlamydiaController(final ChlamydiaService chlamydiaService, final CheckFieldsPatientService checkFieldsPatientService){
        this.chlamydiaService = chlamydiaService;
        this.checkFieldsPatientService = checkFieldsPatientService;
    }

    @PostMapping(value = "/requestPatient")
    @ApiOperation(value = "Get patient's details and return")
    @SneakyThrows
    public ResponseEntity<Request> getQuestions(@ApiParam(value = "Request form containing info about patient", required = true)
                            @RequestBody final Request request) {
        Patient patient = request.getPrefetch().getPatient();
        ChlamydiaPatient chlamydiaPatient = new ChlamydiaPatient(patient.getGender(), patient.getBirthDate(), patient.getId());
        chlamydiaPatientMap.putIfAbsent(patient.getId(), chlamydiaPatient);

        Optional<Questionnaire> questionnaire = checkFieldsPatientService.checkFields(chlamydiaPatientMap.get(patient.getId()),
                    request.getPrefetch().getQuestionnaireResponse());
        if (questionnaire.isPresent()){
            request.getPrefetch().setQuestionnaire(questionnaire.get());
            request.getPrefetch().setQuestionnaireResponse(null);
            request.setCard(null);
        }else{
            Card decision = chlamydiaService.makeDecision(chlamydiaPatientMap.get(patient.getId()));
            request.getPrefetch().setQuestionnaireResponse(null);
            request.setCard(decision);
        }
        return ResponseEntity.status(HttpStatus.OK)
                .body(request);
    }
}