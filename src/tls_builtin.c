/* Create a hash table to track validated certificates */
void* validated_certs = create_hash_table();

for (int i = 0; i < cert_chain_len; i++) {
    /* Skip if certificate already validated */
    if (hash_table_contains(validated_certs, cert_chain[i])) {
        continue;
    }
    
    /* Validate certificate against its parent */
    if (i > 0 && validate_certificate_pair(cert_chain[i-1], cert_chain[i])) {
        hash_table_add(validated_certs, cert_chain[i]);
        /* process validation result */
    }
}

/* Clean up hash table */
free_hash_table(validated_certs);