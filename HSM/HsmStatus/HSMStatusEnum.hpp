namespace HSM {
  /**
   * @enum HSM_STATUS
   * @brief Status for the HSM module.
   */
  enum HSM_STATUS {
    HSM_Good,
    HSM_InvalidAlg,
    HSM_InvalidIdent,
    HSM_NoSuchKey,
    HSM_KeyNotValid,
    HSM_WrongDataSize,
    HSM_BadOutputBufSize,
    HSM_InternalErr,
    // Add more here
    HSM_Status_Count
  };
};