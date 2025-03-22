map_prop_model_order = files('map_prop_models.order')
map_prop_model_file_list = fs.read(map_prop_model_order).splitlines()
map_prop_model_files = copy_gen.process(files(map_prop_model_file_list))

map_prop_model_narc = custom_target('prop_models.narc',
    output: [
        'prop_models.narc',
        'prop_models.naix',
    ],
    input: [
        map_prop_model_files,
        map_prop_model_order,
    ],
    command: [
        narc_exe, 'create',
        '--naix',
        '--order', map_prop_model_order,
        '--output', '@OUTPUT0@',
        '@PRIVATE_DIR@',
    ]
)

nitrofs_files += map_prop_model_narc
