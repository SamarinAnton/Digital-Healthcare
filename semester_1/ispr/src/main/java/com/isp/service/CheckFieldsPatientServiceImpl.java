package com.isp.service;

import com.isp.dto.*;
import lombok.SneakyThrows;
import org.springframework.stereotype.Service;

import java.util.*;


@Service
public class CheckFieldsPatientServiceImpl implements CheckFieldsPatientService {

    private static final List<String> FIELD_NAMES = Arrays.asList("gender", "birthDate",
            "sexuallyActive","isChlamydiaTest",
            "testTime","result",
            "isObserved","observedTime");

    @SneakyThrows
    public Optional<Questionnaire> checkFields(ChlamydiaPatient chlamydiaPatient, QuestionnaireResponse qr) {

        boolean isChosen = false; // TODO not first, search by id in questionaryResponseItems
        chlamydiaPatient.fieldsToArray();
        List<Object> listFields = chlamydiaPatient.getFields();

        for (int i= 0; i < listFields.size(); i++) {
            if (Objects.isNull(listFields.get(i))) {
                if (qr != null && qr.item != null && qr.item.get(0) != null && qr.item.get(0).getAnswer() != null &&
                        qr.item.get(0).getAnswer().get(0) != null && !isChosen) { // TODO get first item
                    Answer answer = qr.item.get(0).getAnswer().get(0);

                    if (Objects.nonNull(answer.getValueString())) {
                        listFields.set(i, answer.getValueString());
                    } else {
                        if (Objects.nonNull(answer.getValueBoolean())) {
                            listFields.set(i, answer.getValueBoolean());
                        } else {
                            if (i == 5) {
                                listFields.set(i, null);
                                isChosen = true;
                                continue;
                            }
                            throw new IllegalArgumentException("Answer must not be null");
                        }
                    }
                    isChosen = true;
                } else {
                    return Optional.of(new Questionnaire(FIELD_NAMES.get(i),
                            Collections.singletonList(new Item(ChlamydiaPatient.questionsMap.get(i).get(0),
                                    ChlamydiaPatient.questionsMap.get(i).get(1)))));
                }
            }
        }

        return Optional.empty();
    }
}
